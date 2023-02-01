#include "../include/Protocol.hpp"
#include "../include/Server.hpp"

// remove after test
// remove after test
// remove after test
// remove after test
#include <iostream>

namespace ft
{
	size_t const Protocol::_message_max_characters = 512;

	size_t const Protocol::_message_max_parameters = 15;

	Protocol::Protocol(Server *server) : _server(server), _commands(), _clients()
	{
		_get_server_operators();
		_commands.insert(std::pair<std::string, fncts>("PASS", &Protocol::cmd_pass));
		_commands.insert(std::pair<std::string, fncts>("NICK", &Protocol::cmd_nick));
		_commands.insert(std::pair<std::string, fncts>("USER", &Protocol::cmd_user));
		_commands.insert(std::pair<std::string, fncts>("PRIVMSG", &Protocol::cmd_privmsg));
		_commands.insert(std::pair<std::string, fncts>("JOIN", &Protocol::cmd_join));
		_commands.insert(std::pair<std::string, fncts>("PING", &Protocol::cmd_ping));
		_commands.insert(std::pair<std::string, fncts>("QUIT", &Protocol::cmd_quit));
		_commands.insert(std::pair<std::string, fncts>("NAMES", &Protocol::cmd_names));
		_commands.insert(std::pair<std::string, fncts>("WHOIS", &Protocol::cmd_whois));
		_commands.insert(std::pair<std::string, fncts>("PART", &Protocol::cmd_part));
		_commands.insert(std::pair<std::string, fncts>("KICK", &Protocol::cmd_kick));
		// _commands.insert(std::pair<std::string, fncts>("INVITE", &Protocol::cmd_invite)); // no mode invite for the channel 
		_commands.insert(std::pair<std::string, fncts>("TOPIC", &Protocol::cmd_topic));
		_commands.insert(std::pair<std::string, fncts>("OPER", &Protocol::cmd_oper));
		_commands.insert(std::pair<std::string, fncts>("MODE", &Protocol::cmd_mode));
		
	}

	Protocol::~Protocol(void) {}

	Protocol::Reply::Reply(Client const& client, std::string const& message)
		: client(client), message(message) {}

	void Protocol::set_password(std::string const &password) { _password = password; }

	void Protocol::add_client(int socket)
	{
		_clients.push_back(Client(socket));
	}

	void Protocol::delete_client(int socket)
	{
		for (size_t i = 0; i < _clients.size(); i++)
		{
			if (_clients[i].get_socket() == socket)
				_clients.erase(_clients.begin() + i);
		}
	}

	void Protocol::parse_client_input(std::string &client_msg, int client_socket)
	{
		std::string line;
		size_t pos;
		std::vector<std::string> lines;

		// if message is > 512 chars, we truncate it and add CRLF at the end
		if (client_msg.size() > _message_max_characters)
		{
			client_msg.resize(_message_max_characters - 2);
			client_msg.append("\r\n");
		}
		do
		{
			pos = client_msg.find("\r\n");
			line = client_msg.substr(0, pos);
			// we do not care about empty messages
			std::cout << "line = " << line << std::endl; // TEST TEST TEST TEST TEST
			if (!line.empty())
				lines.push_back(line);
			client_msg.erase(0, pos + 2);
		} while (pos != std::string::npos);
		for (size_t i = 0; i < lines.size(); i++)
		{
			Message msg(lines[i], client_socket);
			// we do not care about messages with more than 15 parameters
			if (msg.get_parameters().size() <= _message_max_parameters)
				handle_message(msg);
		}
		send_replies();
		_queue.clear();
		_ignored_sockets.clear();
	}

	void Protocol::handle_message(Message msg)
	{
		if (_commands.count(msg.get_command()) && !is_socket_ignored(msg.get_socket()))
			(this->*_commands[msg.get_command()])(msg);
	}

	void Protocol::ignore_socket(int socket)
	{
		if (!is_socket_ignored(socket))
			_ignored_sockets.push_back(socket);
	}

	bool Protocol::is_socket_ignored(int socket) const
	{
		std::vector<int>::const_iterator cit;

		for (cit = _ignored_sockets.begin(); cit != _ignored_sockets.end(); cit++)
		{
			if (*cit == socket)
				return true;
		}
		return false;
	}

	void Protocol::add_to_queue(Client const& client, std::string const& message, int index)
	{
		Reply r(client, message);
		_queue.push_back(r);
		if (!index)
			ignore_socket(client.get_socket());
	}

	void Protocol::send_replies(void)
	{
		std::vector<Reply>::const_iterator cit;
		ssize_t x;

		for (cit = _queue.begin(); cit != _queue.end(); cit++)
		{
			x = send((*cit).client.get_socket(), (*cit).message.c_str(), (*cit).message.size(), 0);
			if (x == -1)
				_server->close_socket_connection((*cit).client.get_socket());
		}
	}

	bool Protocol::_is_client_connected(Client client) const
	{
		if (client.get_nickname_status() && client.get_registration_status()
				&& client.get_password_status())
			return true;
		return false;
	}

	bool Protocol::_is_nickname_taken(std::string const& nickname) const
	{
		for (std::vector<Client>::const_iterator cit = _clients.begin(); cit != _clients.end(); cit++)
		{
			if (cit->get_nickname() == nickname)
				return true;
		}
		return false;
	}

	Client &Protocol::_get_client_from_socket(int socket)
	{
		size_t pos;

		for (size_t i = 0; i < _clients.size(); i++)
		{
			if (_clients[i].get_socket() == socket)
				pos = i;
		}
		return _clients[pos];
	}

	Client &Protocol::_get_client_from_nickname(const std::string &nickname)
	{
		size_t pos;

		for (size_t i = 0; i < _clients.size(); i++)
		{
			if (_clients[i].get_nickname() == nickname)
				pos = i;
		}

		// Return a reference to the client at the found position
		return _clients[pos];
	}
	// void Protocol::add_channel(std::string channel_name)
	// {
	// 	_channels.push_back(Channel(channel_name));
	// }

	void Protocol::delete_channel(std::string channel_name)
	{
		for (size_t i = 0; i < _channels.size(); i++)
		{
			if (_channels[i]->get_name() == channel_name)
				_channels.erase(_channels.begin() + i);
		}
	}

	bool Protocol::is_valid_channel_name(std::string channel_name)
	{
		// check if the channel name starts with '#'
		if (channel_name.size() < 1)
			return false;
		if (channel_name[0] != '#')
			return false;

		// check if the channel name contains only valid characters
		for (unsigned int i = 0; i < channel_name.size(); i++)
		{
			char c = channel_name[i];
			if (!isalnum(c) && c != '#' && c != '-' && c != '_')
				return false;
		}

		return true;
	}

	bool Protocol::_channel_exists(std::string const& channel) const
	{
		for (size_t i = 0; i < _channels.size(); i++)
		{
			if (_channels[i]->get_name() == channel)
				return true;
		}
		return false;
	}

	Channel* Protocol::_get_channel_from_name(const std::string &channel_name)
	{
		for (size_t i = 0; i < _channels.size(); i++)
		{
			if (_channels[i]->get_name() == channel_name)
				return _channels[i];
		}
		throw std::out_of_range("channel not found");
	}

	// no data validation or parsing -> might be an issue
	void Protocol::_get_server_operators(void)
	{
		std::ifstream env_file;
		std::vector<std::string> words;
		std::string line;
		std::string word;

		env_file.open(".env", std::ios::in);
		// if (!env_file)
			// error
		while (std::getline(env_file, line))
		{
			std::stringstream ss(line);

			while (std::getline(ss, word, '='))
				words.push_back(word);
		}
		env_file.close();
		for (size_t i = 0; i < words.size(); i += 2)
			_server_ops.insert(std::make_pair(words[i], words[i + 1]));
	}

	bool Protocol::_is_valid_mode(std::string const& str, std::string const& modes) const
	{
		if (str.size() != 2 || str.find_first_of("+-") != 0 || str.find_first_of(modes) != 1)
			return false;
		return true;
	}

	void Protocol::_send_welcome_messages(Client const& client)
	{
		std::string reply;

		reply = rpl_welcome(client.get_nickname(), "42FT_IRC", client.get_nickname(),
				client.get_username(), client.get_hostname());
		add_to_queue(client, reply, 1);
		reply = rpl_yourhost(client.get_nickname(), client.get_hostname(), _server->get_version());
		add_to_queue(client, reply, 1);
		reply = rpl_created(client.get_nickname(), _server->get_creation_time());
		add_to_queue(client, reply, 1);
		reply = rpl_myinfo(client.get_nickname(), client.get_servername(), _server->get_version(),
			   "TEMP VALUES", "TEMP VALUES", "TEMP VALUES");
		add_to_queue(client, reply, 1);
	}

	void Protocol::send_msg_to_channel_clients(Channel const& channel, Client const* client,
			std::string const& message)
	{
		std::vector<Client*>::const_iterator cit;

		for (cit = channel.get_clients().begin(); cit != channel.get_clients().end(); cit++)
		{
			// no need to check if client == NULL as *cit will never == NULL
			if (client != *cit)
				add_to_queue(*(*cit), message, 1);
		}
	}
}
