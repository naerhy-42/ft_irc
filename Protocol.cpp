#include "Protocol.hpp"

// remove after test
// remove after test
// remove after test
// remove after test
#include <iostream>

namespace ft
{
	size_t const Protocol::_message_max_characters = 512;

	size_t const Protocol::_message_max_parameters = 15;

	Protocol::Protocol(void) : _functions(), _clients()
	{
		_functions.insert(std::pair<std::string, fncts>("PASS", &Protocol::pass_function));
		_functions.insert(std::pair<std::string, fncts>("NICK", &Protocol::nick_function));
		_functions.insert(std::pair<std::string, fncts>("USER", &Protocol::user_function));
	}

	Protocol::~Protocol(void) {}

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
		size_t pos;

		parameters = msg.get_parameters();
		if (parameters.empty())
			// return ERR_NEEDMOREPARAMS
			return ;
		// if (parameters.at(0) == server password)
			// return ERR_PASSWDMISMATCH
			return ;
		pos = _get_client_pos_from_socket(msg.get_socket());
		// check if user is already registered
		if (_clients[pos].get_registration_status())
			// return ERR_ALREADYREGISTERED
			return ;
		_clients[pos].set_password_status(true);
	}

	void Protocol::nick_function(Message msg)
	{
		std::vector<std::string> parameters;
		std::string nickname;
		size_t pos;

		parameters = msg.get_parameters();
		// check if parameter exist and if nickname is not empty
		if (parameters.empty() || parameters.at(0).empty())
			// return ERR_NONICKNAMEGIVEN
			return ;
		else
			nickname = parameters.at(0);
		// check if nickname contains only valid characters and contains less than 9 characters
		if (nickname.length() > 9 || nickname.find_first_of(" ,*?!@.#&()[]") != std::string::npos)
			// return ERR_ERRONEUSNICKNAME
			return ;
		// check if nickname is free
		for (std::vector<Client>::const_iterator cit = _clients.begin(); cit != _clients.end(); cit++)
		{
			if (cit->get_nickname() == nickname)
				// return ERR_NICKNAMEINUSE
				return ;
		}
		pos = _get_client_pos_from_socket(msg.get_socket());
		_clients[pos].set_nickname(nickname);
		// return replies to other members if needed [?]
	}

	void Protocol::user_function(Message msg)
	{
		std::vector<std::string> parameters;
		size_t pos;

		parameters = msg.get_parameters();
		if (parameters.size() < 4 || parameters.at(0).empty() || parameters.at(1).empty()
				|| parameters.at(2).empty() || parameters.at(3).empty())
			// return ERR_NEEDMOREPARAMS
			return ;
		pos = _get_client_pos_from_socket(msg.get_socket());
		// check if user is already registered
		if (_clients[pos].get_registration_status())
			// return ERR_ALREADYREGISTERED
			return ;
		_clients[pos].set_username(parameters.at(0));
		_clients[pos].set_hostname(parameters.at(1));
		// we do not store servername because useless if we don't handle multi server [?]
		_clients[pos].set_real_name(msg.get_remainder());
		_clients[pos].set_registration_status(true);
		if (!_clients[pos].get_password_status() || !_clients[pos].get_registration_status())
			// return ERR - but which one??
			return ;
		// return RPL_WELCOME
	}

	size_t Protocol::_get_client_pos_from_socket(int socket)
	{
		size_t i = 0;
		for (i = 0; i < _clients.size(); i++)
		{
			if (_clients[i].get_socket() == socket)
				return i;
		}
		return -1;
	}
}
