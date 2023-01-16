#include "Protocol.hpp"
#include "Server.hpp"

// remove after test
// remove after test
// remove after test
// remove after test
#include <iostream>

namespace ft
{
	size_t const Protocol::_message_max_characters = 512;

	size_t const Protocol::_message_max_parameters = 15;

	Protocol::Protocol(Server const *server) : _server(server), _commands(), _clients()
	{
		_commands.insert(std::pair<std::string, fncts>("PASS", &Protocol::cmd_pass));
		_commands.insert(std::pair<std::string, fncts>("NICK", &Protocol::cmd_nick));
		_commands.insert(std::pair<std::string, fncts>("USER", &Protocol::cmd_user));
		_commands.insert(std::pair<std::string, fncts>("PRIVMSG", &Protocol::cmd_privmsg));
		_commands.insert(std::pair<std::string, fncts>("JOIN", &Protocol::cmd_join));
	}

	Protocol::~Protocol(void) {}

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
	}

	void Protocol::handle_message(Message msg)
	{
		if (_commands.count(msg.get_command()))
			(this->*_commands[msg.get_command()])(msg);
		// else do something if command is unknown??
	}

	void Protocol::cmd_pass(Message msg)
	{
		std::vector<std::string> parameters;
		std::string error;
		Client &client = _get_client_from_socket(msg.get_socket());

		if (client.get_password_status())
		{
			error = err_alreadyregistered(client.get_nickname());
			send(msg.get_socket(), error.c_str(), error.size(), 0);
			return;
		}
		parameters = msg.get_parameters();
		if (parameters.empty())
		{
			error = err_needmoreparams(client.get_nickname(), "PASS");
			send(msg.get_socket(), error.c_str(), error.size(), 0);
			return;
		}
		if (parameters[0] != _password)
		{
			error = err_passwdmismatch(client.get_nickname());
			send(msg.get_socket(), error.c_str(), error.size(), 0);
			return;
		}
		client.set_password_status(true);
	}

	void Protocol::cmd_nick(Message msg)
	{
		std::vector<std::string> parameters;
		std::string nickname;
		std::string error;
		Client &client = _get_client_from_socket(msg.get_socket());

		if ((client.get_nickname() == "*" && !client.get_password_status()) || (client.get_nickname() != "*" && !client.get_registration_status()))
		{
			error = err_notregistered(client.get_nickname());
			send(msg.get_socket(), error.c_str(), error.size(), 0);
			return;
		}
		parameters = msg.get_parameters();
		if (parameters.empty() || parameters[0].empty())
		{
			error = err_nonicknamegiven(client.get_nickname());
			send(msg.get_socket(), error.c_str(), error.size(), 0);
			return;
		}
		else
			nickname = parameters[0];
		if (nickname.length() > 9 || nickname.find_first_of(" ,*?!@.#&()[]") != std::string::npos)
		{
			error = err_erroneusnickname(client.get_nickname(), nickname);
			send(msg.get_socket(), error.c_str(), error.size(), 0);
			return;
		}
		for (std::vector<Client>::const_iterator cit = _clients.begin(); cit != _clients.end(); cit++)
		{
			if (cit->get_nickname() == nickname)
			{
				error = err_nicknameinuse(client.get_nickname(), nickname);
				send(msg.get_socket(), error.c_str(), error.size(), 0);
				return;
			}
		}
		client.set_nickname(nickname);
		// return replies to other members if needed [?]
	}

	void Protocol::cmd_user(Message msg)
	{
		std::vector<std::string> parameters;
		std::string error;
		std::string reply;
		Client &client = _get_client_from_socket(msg.get_socket());

		if (!client.get_password_status())
		{
			error = err_notregistered(client.get_nickname());
			send(msg.get_socket(), error.c_str(), error.size(), 0);
			return;
		}
		if (client.get_nickname() == "*")
		{
			error = err_nonicknamegiven(client.get_nickname());
			send(msg.get_socket(), error.c_str(), error.size(), 0);
			return;
		}
		parameters = msg.get_parameters();

		if (parameters.size() < 3 || parameters[0].empty() || parameters[1].empty() || parameters[2].empty() || msg.get_remainder().empty())
		{
			error = err_needmoreparams(client.get_nickname(), "USER");
			send(msg.get_socket(), error.c_str(), error.size(), 0);
			return;
		}
		if (client.get_registration_status())
		{
			error = err_alreadyregistered(client.get_nickname());
			send(msg.get_socket(), error.c_str(), error.size(), 0);
			return;
		}
		client.set_username(parameters[0]);
		client.set_hostname(parameters[1]);
		client.set_servername(parameters[2]);
		client.set_real_name(msg.get_remainder());
		client.set_registration_status(true);
		reply = rpl_welcome(client.get_nickname(), "42FT_IRC", client.get_nickname(),
							client.get_username(), client.get_hostname());
		send(msg.get_socket(), reply.c_str(), reply.size(), 0);
		reply = rpl_yourhost(client.get_nickname(), client.get_hostname(), _server->get_version());
		send(msg.get_socket(), reply.c_str(), reply.size(), 0);
		reply = rpl_created(client.get_nickname(), _server->get_creation_time());
		send(msg.get_socket(), reply.c_str(), reply.size(), 0);
		reply = rpl_myinfo(client.get_nickname(), client.get_servername(), _server->get_version(),
						   "TEMP VALUES", "TEMP VALUES", "TEMP VALUES");
		send(msg.get_socket(), reply.c_str(), reply.size(), 0);
	}

	// void Protocol::cmd_quit(Message msg)
	// {
	// 	Client& client = _get_client_from_socket(msg.get_socket());

	// 	// IRC clients should normally not send those commands if unregistered...
	// 	if (!client.get_password_status())
	// 		return;
	// 	delete_client(client.get_socket());
	// 	// call server function to delete client socket from _fds
	// 	// _server is const => unable to call this function, remove const??
	// 	// _server.remove_socket(client.get_socket());
	// 	// if client is on a channel, send QUIT command to clients in this channel
	// }

	// void Protocol::cmd_quit(Message msg)
	// {
	// }

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

	void Protocol::cmd_privmsg(Message msg)
	{
		// Get the client sending the message
		Client &current_client = _get_client_from_socket(msg.get_socket());

		// Extract the parameters from the message
		std::vector<std::string> parameters = msg.get_parameters();

		// Ensure that the message has at least one parameter (the target)
		if (parameters.size() < 1)
		{
			// If there is no target parameter, send an error message
			std::string error = err_needmoreparams(current_client.get_nickname(), "PRIVMSG");
			send(msg.get_socket(), error.c_str(), error.size(), 0);
			return;
		}

		// Determine if the target is a user or a channel
		bool is_channel = false;
		if (parameters[0][0] == '#')
		{
			is_channel = true;
		}

		// Extract the message text from the parameters
		std::string message = msg.get_remainder();

		// Send the message to the target/channel
		if (is_channel)
		{
			// Check if the channel exists
			std::string const target_channel_name = parameters[0];
			bool channel_exists = false;
			for (size_t i = 0; i < _channels.size(); i++)
			{
				if (_channels[i].get_name() == target_channel_name)
				{
					channel_exists = true;
					break;
				}
			}
			if (channel_exists)
			{
				// Send the message to all clients in the channel
				Channel &target_channel = _get_channel_from_name(target_channel_name);
				std::string send_msg = ":" + current_client.get_nickname() + " PRIVMSG " + target_channel.get_name() + " :" + message + "\r\n";
				for (size_t i = 0; i < target_channel.get_clients().size(); i++)
				{
					int client_socket = target_channel.get_clients()[i].get_socket();
					if (client_socket != current_client.get_socket())
						send(client_socket, send_msg.c_str(), send_msg.size(), 0);
				}
				return;
			}
			else
			{
				// If there is no target channel, send an error message
				std::string error = err_nosuchchannel(current_client.get_nickname(), target_channel_name);
				send(msg.get_socket(), error.c_str(), error.size(), 0);
				return;
			}
		}
		else
		{
			// Check if the nickname exists
			std::string const target_nickname = parameters[0];
			bool nickname_exists = false;
			for (size_t i = 0; i < _clients.size(); i++)
			{
				if (_clients[i].get_nickname() == target_nickname)
				{
					nickname_exists = true;
					break;
				}
			}
			if (nickname_exists)
			{
				// Send the message if the target exists
				Client &target_client = _get_client_from_nickname(target_nickname);
				std::string send_msg = ":" + current_client.get_nickname() + " PRIVMSG " + target_nickname + " :" + message + "\r\n";
				send(target_client.get_socket(), send_msg.c_str(), send_msg.size(), 0);
				return;
			}
			else
			{
				// If there is no target nickname, send an error message
				std::string error = err_nosuchnick(current_client.get_nickname(), target_nickname);
				send(msg.get_socket(), error.c_str(), error.size(), 0);
				return;
			}
		}
	}

	void Protocol::add_channel(std::string channel_name)
	{
		_channels.push_back(Channel(channel_name));
	}

	void Protocol::delete_channel(std::string channel_name)
	{
		for (size_t i = 0; i < _channels.size(); i++)
		{
			if (_channels[i].get_name() == channel_name)
				_channels.erase(_channels.begin() + i);
		}
	}

	bool Protocol::is_valid_channel_name(std::string channel_name)
	{
		// check if the channel name starts with '#'
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

	Channel &Protocol::_get_channel_from_name(const std::string &channel_name)
	{
		size_t pos;

		for (size_t i = 0; i < _channels.size(); i++)
		{
			if (_channels[i].get_name() == channel_name)
				pos = i;
		}
		return _channels[pos];
	}

	void Protocol::cmd_join(Message msg)
	{
		// Get the client joining the channel
		Client &current_client = _get_client_from_socket(msg.get_socket());

		// Extract the parameters from the message
		std::vector<std::string> parameters = msg.get_parameters();

		// Ensure that the message has at least one parameter (the channel name)
		if (parameters.size() < 1)
		{
			// If there is no channel parameter, send an error message
			std::string error = err_needmoreparams(current_client.get_nickname(), "JOIN");
			send(msg.get_socket(), error.c_str(), error.size(), 0);
			return;
		}

		// Extract the channel name from the parameters
		std::string channel_name = parameters[0];

		// Check if the channel already exists
		bool channel_exists = false;
		for (size_t i = 0; i < _channels.size(); i++)
		{
			if (_channels[i].get_name() == channel_name)
			{
				channel_exists = true;
				break;
			}
		}

		if (channel_exists)
		{
			// Add the client to the channel if it exists
			Channel &channel = _get_channel_from_name(channel_name);
			channel.add_client(current_client);
		}
		else
		{
			bool is_channel_name_corect = is_valid_channel_name(channel_name);
			if (is_channel_name_corect == true)
			{
				// Create a new channel and add the client to it if it doesn't exist
				Channel new_channel(channel_name);
				new_channel.add_client(current_client);
				_channels.push_back(new_channel);
			}
			else
			{
				// this error does not exist in protocol
			}
		}

		// Send a JOIN message to the client
		std::string join_msg = ":" + current_client.get_nickname() + " JOIN " + channel_name + "\r\n";
		send(current_client.get_socket(), join_msg.c_str(), join_msg.size(), 0);
	}

}
