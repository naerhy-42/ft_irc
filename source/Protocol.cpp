#include "Protocol.hpp"
#include "Server.hpp"

namespace ft
{
	size_t const Protocol::_MESSAGE_MAX_CHARACTERS = 512;

	size_t const Protocol::_MESSAGE_MAX_PARAMETERS = 15;

	std::string const Protocol::_IRC_ENDL = "\r\n";

	Protocol::Protocol(Server &server, std::string const &hostname)
		: _server(server), _replies(":" + hostname, _IRC_ENDL), _user_modes("io"),
		_channel_modes("mt"), _user_chan_modes("ov"), _SERVER_RUNNING(true)
	{
		_commands.insert(std::pair<std::string, fncts>("JOIN", &Protocol::cmd_join));
		_commands.insert(std::pair<std::string, fncts>("KICK", &Protocol::cmd_kick));
		_commands.insert(std::pair<std::string, fncts>("MODE", &Protocol::cmd_mode));
		_commands.insert(std::pair<std::string, fncts>("NICK", &Protocol::cmd_nick));
		_commands.insert(std::pair<std::string, fncts>("OPER", &Protocol::cmd_oper));
		_commands.insert(std::pair<std::string, fncts>("PART", &Protocol::cmd_part));
		_commands.insert(std::pair<std::string, fncts>("PASS", &Protocol::cmd_pass));
		_commands.insert(std::pair<std::string, fncts>("PING", &Protocol::cmd_ping));
		_commands.insert(std::pair<std::string, fncts>("PRIVMSG", &Protocol::cmd_privmsg));
		_commands.insert(std::pair<std::string, fncts>("TOPIC", &Protocol::cmd_topic));
		_commands.insert(std::pair<std::string, fncts>("QUIT", &Protocol::cmd_quit));
		_commands.insert(std::pair<std::string, fncts>("WHOIS", &Protocol::cmd_whois));
		_commands.insert(std::pair<std::string, fncts>("USER", &Protocol::cmd_user));
		_commands.insert(std::pair<std::string, fncts>("die", &Protocol::cmd_die));
		/*
		_commands.insert(std::pair<std::string, fncts>("INVITE", &Protocol::cmd_invite)); // no mode invite for the channel
		_commands.insert(std::pair<std::string, fncts>("NAMES", &Protocol::cmd_names));
		*/
	}

	Protocol::~Protocol(void)
	{
		std::vector<Client *>::iterator it;

		for (it = _clients.begin(); it != _clients.end(); it++)
			delete *it;
		// no need to use _clients.clear() in order to remove dangling pointers as
		// Protocol is only destroyed at end of program
	}

	Client *Protocol::get_client_from_socket(int socket)
	{
		std::vector<Client *>::iterator it;

		for (it = _clients.begin(); it != _clients.end(); it++)
		{
			if ((*it)->get_socket() == socket)
				return *it;
		}
		return NULL;
	}

	Client *Protocol::get_client_from_name(std::string const &name)
	{
		std::vector<Client *>::iterator it;

		for (it = _clients.begin(); it != _clients.end(); it++)
		{
			if ((*it)->get_nickname() == name)
				return *it;
		}
		return NULL;
	}

	Channel &Protocol::get_channel_from_name(std::string const &name)
	{
		size_t pos = 0;

		for (size_t i = 0; i < _channels.size(); i++)
		{
			if (_channels[i].get_name() == name)
			{
				pos = i;
				break;
			}
		}
		return _channels[pos];
	}

	std::string Protocol::get_enabled_modes(int id) const
	{
		if (!id)
			return _user_modes;
		if (id == 1)
			return _channel_modes;
		else
			return _user_chan_modes;
	}

	std::string Protocol::get_user_channels_list(Client const *client) const
	{
		std::string user_channels_str;
		std::vector<Channel>::const_iterator cit;

		for (cit = _channels.begin(); cit != _channels.end(); cit++)
		{
			if ((*cit).has_client(client))
			{
				if (user_channels_str.empty())
					user_channels_str += (*cit).get_client_prefix(client) + (*cit).get_name();
				else
					user_channels_str += " " + (*cit).get_client_prefix(client) + (*cit).get_name();
			}
		}
		return user_channels_str;
	}

	bool Protocol::get_server_status(void) const
	{
		return _SERVER_RUNNING;
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

	bool Protocol::is_client_connected(Client const *client) const
	{
		if (client->get_password_status() && client->get_nickname_status() && client->get_registration_status())
			return true;
		return false;
	}

	bool Protocol::is_client_active(std::string const &client_name) const
	{
		std::vector<Client *>::const_iterator cit;

		for (cit = _clients.begin(); cit != _clients.end(); cit++)
		{
			if ((*cit)->get_nickname() == client_name)
				return true;
		}
		return false;
	}

	bool Protocol::is_channel_active(std::string const &channel_name) const
	{
		std::vector<Channel>::const_iterator cit;

		for (cit = _channels.begin(); cit != _channels.end(); cit++)
		{
			if ((*cit).get_name() == channel_name)
				return true;
		}
		return false;
	}

	bool Protocol::is_valid_nickname(std::string const &nickname) const
	{
		std::string::const_iterator cit;

		if (nickname.size() > 18 || nickname.size() < 2)
			return false;
		for (cit = nickname.begin(); cit != nickname.end(); cit++)
		{
			if (!isalnum(*cit) && *cit != '-' && *cit != '_')
				return false;
		}
		return true;
	}

	bool Protocol::is_channel_name(std::string const &name) const
	{
		if (name.size() > 1 && name[0] == '#')
			return true;
		return false;
	}

	bool Protocol::is_valid_mode(std::string const &mode, int id) const
	{
		std::string enabled_modes = get_enabled_modes(id);

		if (mode.size() != 2 || mode.find_first_of("+-") || mode.find_first_of(enabled_modes) != 1)
			return false;
		return true;
	}

	void Protocol::set_password(std::string const &password) { _password = password; }

	void Protocol::set_global_operators(std::vector<std::string> const &operators)
	{
		std::vector<std::string>::const_iterator cit;

		for (cit = operators.begin(); cit != operators.end(); cit += 2)
			_global_operators.insert(std::make_pair(*cit, *(cit + 1)));
	}

	void Protocol::add_client(int socket) { _clients.push_back(new Client(socket)); }

	void Protocol::delete_client(int socket)
	{
		std::vector<Client *>::iterator it;

		for (it = _clients.begin(); it != _clients.end(); it++)
		{
			if ((*it)->get_socket() == socket)
			{
				delete *it;
				_clients.erase(it);
				break;
			}
		}
	}

	void Protocol::add_channel(std::string const &name, Client *client)
	{
		_channels.push_back(Channel(name, client));
	}

	void Protocol::delete_channel(std::string const &name)
	{
		std::vector<Channel>::iterator it;

		for (it = _channels.begin(); it != _channels.end(); it++)
		{
			if ((*it).get_name() == name)
			{
				_channels.erase(it);
				break;
			}
		}
	}

	void Protocol::remove_client_from_channels(Client const *client)
	{
		std::vector<Channel>::iterator it;
		std::vector<std::vector<Channel>::iterator> channels_to_delete;
		std::vector<std::vector<Channel>::iterator>::iterator itt;

		for (it = _channels.begin(); it != _channels.end(); it++)
		{
			if ((*it).has_client(client))
			{
				(*it).remove_client(client);
				if (!(*it).get_clients().size())
					channels_to_delete.push_back(it);
			}
		}
		for (itt = channels_to_delete.begin(); itt != channels_to_delete.end(); itt++)
			_channels.erase(*itt);
	}

	void Protocol::ignore_socket(int socket)
	{
		if (!is_socket_ignored(socket))
			_ignored_sockets.push_back(socket);
	}

	void Protocol::parse_client_input(int socket, std::string &message)
	{
		std::string line;
		size_t pos;
		std::vector<std::string> lines;
		std::vector<std::string>::iterator it;

		if (message.size() > _MESSAGE_MAX_CHARACTERS)
		{
			message.resize(_MESSAGE_MAX_CHARACTERS - 2);
			message.append(_IRC_ENDL);
		}
		do
		{
			pos = message.find(_IRC_ENDL);
			line = message.substr(0, pos);
			// debugging:
			std::cout << "line = " << line << std::endl;
			if (!line.empty())
				lines.push_back(line);
			message.erase(0, pos + 2);
		} while (pos != std::string::npos);
		for (it = lines.begin(); it != lines.end(); it++)
		{
			ClientMessage cmessage(get_client_from_socket(socket), *it);

			if (cmessage.get_parameters().size() <= _MESSAGE_MAX_PARAMETERS)
				handle_message(cmessage);
		}
		_ignored_sockets.clear();
	}

	void Protocol::handle_message(ClientMessage const &cmessage)
	{
		std::string const &command = cmessage.get_command();

		if (_commands.count(command) && !is_socket_ignored(cmessage.get_client()->get_socket()))
			(this->*_commands[command])(cmessage);
	}

	void Protocol::send_message_to_client(Client const *client, std::string const &message)
	{
		// disconnect the user if send return -1 ?
		if (send(client->get_socket(), message.c_str(), message.size(), 0) == -1)
			std::cout << "Could not write to socket..." << std::endl;
	}

	void Protocol::send_message_to_channel(Channel const &channel, std::string const &message,
										   Client const *sender)
	{
		std::map<Client const *, Modes> const &clients = channel.get_clients();
		std::map<Client const *, Modes>::const_iterator cit;

		for (cit = clients.begin(); cit != clients.end(); cit++)
		{
			if (cit->first != sender)
				send_message_to_client(cit->first, message);
		}
	}

	void Protocol::send_message_to_client_channels(Client const *client, std::string const &message)
	{
		std::vector<Channel>::const_iterator cit;

		for (cit = _channels.begin(); cit != _channels.end(); cit++)
		{
			if ((*cit).has_client(client))
				send_message_to_channel(*cit, message, client);
		}
	}

	void Protocol::send_welcome_messages(Client const *client)
	{
		send_message_to_client(client, _replies.rpl_welcome(client->get_nickname(),
															client->get_prefix()));
		send_message_to_client(client, _replies.rpl_yourhost(client->get_nickname(),
															 _server.get_hostname(), _server.get_version()));
		send_message_to_client(client, _replies.rpl_created(client->get_nickname(),
															_server.get_creation_time()));
		send_message_to_client(client, _replies.rpl_myinfo(client->get_nickname(),
														   _server.get_hostname(), _server.get_version(),
														   _user_modes, _channel_modes, _user_chan_modes));
	}
}
