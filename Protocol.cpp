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

	Protocol::Protocol(Server const* server) : _server(server), _functions(), _clients()
	{
		_functions.insert(std::pair<std::string, fncts>("PASS", &Protocol::pass_function));
		_functions.insert(std::pair<std::string, fncts>("NICK", &Protocol::nick_function));
		_functions.insert(std::pair<std::string, fncts>("USER", &Protocol::user_function));
	}

	Protocol::~Protocol(void) {}

	void Protocol::set_password(std::string const& password) { _password = password; }

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

	void Protocol::parse_client_input(std::string& client_msg, int client_socket)
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
		if (_functions.count(msg.get_command()))
			(this->*_functions[msg.get_command()])(msg);
		// else do something if command is unknown??
	}

	void Protocol::pass_function(Message msg)
	{
		std::vector<std::string> parameters;
		std::string error;
		Client& client = _get_client_from_socket(msg.get_socket());

		if (client.get_password_status())
		{
			error = err_alreadyregistered(client.get_nickname());
			send(msg.get_socket(), error.c_str(), error.size(), 0);
			return ;
		}
		parameters = msg.get_parameters();
		if (parameters.empty())
		{
			error = err_needmoreparams(client.get_nickname(), "PASS");
			send(msg.get_socket(), error.c_str(), error.size(), 0);
			return ;
		}
		if (parameters[0] != _password)
		{
			error = err_passwdmismatch(client.get_nickname());
			send(msg.get_socket(), error.c_str(), error.size(), 0);
			return ;
		}
		client.set_password_status(true);
	}

	void Protocol::nick_function(Message msg)
	{
		std::vector<std::string> parameters;
		std::string nickname;
		std::string error;
		Client& client = _get_client_from_socket(msg.get_socket());

		if ((client.get_nickname() == "*" && !client.get_password_status())
				|| (client.get_nickname() != "*" && !client.get_registration_status()))
		{
			error = err_notregistered(client.get_nickname());
			send(msg.get_socket(), error.c_str(), error.size(), 0);
			return ;
		}
		parameters = msg.get_parameters();
		if (parameters.empty() || parameters[0].empty())
		{
			error = err_nonicknamegiven(client.get_nickname());
			send(msg.get_socket(), error.c_str(), error.size(), 0);
			return ;
		}
		else
			nickname = parameters[0];
		if (nickname.length() > 9 || nickname.find_first_of(" ,*?!@.#&()[]") != std::string::npos)
		{
			error = err_erroneusnickname(client.get_nickname(), nickname);
			send(msg.get_socket(), error.c_str(), error.size(), 0);
			return ;
		}
		for (std::vector<Client>::const_iterator cit = _clients.begin(); cit != _clients.end(); cit++)
		{
			if (cit->get_nickname() == nickname)
			{
				error = err_nicknameinuse(client.get_nickname(), nickname);
				send(msg.get_socket(), error.c_str(), error.size(), 0);
				return ;
			}
		}
		client.set_nickname(nickname);
		// return replies to other members if needed [?]
	}

	void Protocol::user_function(Message msg)
	{
		std::vector<std::string> parameters;
		std::string error;
		std::string reply;
		Client& client = _get_client_from_socket(msg.get_socket());

		if (!client.get_password_status())
		{
			error = err_notregistered(client.get_nickname());
			send(msg.get_socket(), error.c_str(), error.size(), 0);
			return ;
		}
		if (client.get_nickname() == "*")
		{
			error = err_nonicknamegiven(client.get_nickname());
			send(msg.get_socket(), error.c_str(), error.size(), 0);
			return ;
		}
		parameters = msg.get_parameters();

		if (parameters.size() < 3 || parameters[0].empty() || parameters[1].empty()
				|| parameters[2].empty() || msg.get_remainder().empty())
		{
			error = err_needmoreparams(client.get_nickname(), "USER");
			send(msg.get_socket(), error.c_str(), error.size(), 0);
			return ;
		}
		if (client.get_registration_status())
		{
			error = err_alreadyregistered(client.get_nickname());
			send(msg.get_socket(), error.c_str(), error.size(), 0);
			return ;
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

	Client& Protocol::_get_client_from_socket(int socket)
	{
		size_t pos;

		for (size_t i = 0; i < _clients.size(); i++)
		{
			if (_clients[i].get_socket() == socket)
				pos = i;
		}
		return _clients[pos];
	}
}
