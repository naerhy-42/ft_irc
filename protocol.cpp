#include "protocol.hpp"

// remove after test
// remove after test
// remove after test
// remove after test
#include <iostream>

namespace ft
{
	size_t const protocol::_message_max_characters = 512;

	size_t const protocol::_message_max_parameters = 15;

	protocol::protocol(void) : _functions(), _clients()
	{
		_functions.insert(std::pair<std::string, fncts>("NICK", &protocol::nick_function));
		_functions.insert(std::pair<std::string, fncts>("USER", &protocol::user_function));
	}

	protocol::~protocol(void) {}

	void protocol::add_client(int socket)
	{
		_clients.push_back(client(socket));
	}

	void protocol::delete_client(int socket)
	{
		for (size_t i = 0; i < _clients.size(); i++)
		{
			if (_clients[i].get_socket() == socket)
				_clients.erase(_clients.begin() + i);
		}
	}

	void protocol::parse_client_input(std::string& client_msg, int client_socket)
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
			if (!line.empty())
				lines.push_back(line);
			client_msg.erase(0, pos + 2);
		} while (pos != std::string::npos);
		for (size_t i = 0; i < lines.size(); i++)
		{
			message msg(lines[i], client_socket);
			// we do not care about messages with more than 15 parameters
			if (msg.get_parameters().size() <= _message_max_parameters)
				handle_message(msg);
		}
	}

	void protocol::handle_message(message msg)
	{
		if (_functions.count(msg.get_command()))
			(this->*_functions[msg.get_command()])(msg);
		// else do something if command is unknown??
	}

	void protocol::nick_function(message msg)
	{
		size_t pos;
		std::string reply; // find a more appropriate name

		std::cout << "nick function has been called" << std::endl;
		// check if parameter in message in not empty
		// check if nickname contains only valid characters
		// check if nickname is free
		pos = _get_client_pos_from_socket(msg.get_socket());
		_clients[pos].set_nickname(msg.get_parameters()[0]);
		// return replies to other members if needed
		// send(_clients[pos].get_socket(), reply.c_str(), reply.size(), 0);
	}

	void protocol::user_function(message msg)
	{
		size_t pos;
		std::string reply; // find a more appropriate name

		std::cout << "user function has been called" << std::endl;
		pos = _get_client_pos_from_socket(msg.get_socket());
		_clients[pos].set_username(msg.get_parameters()[0]);
		_clients[pos].set_hostname(msg.get_parameters()[1]);
		// we do not store servername because useless if we don't handle multi server?
		_clients[pos].set_real_name(msg.get_remainder());
		reply += ":localhost 001 " + _clients[pos].get_nickname() + " :Welcome to the Internet Relay \
			  Network " + _clients[pos].get_nickname() + "!" + _clients[pos].get_username()
			  + "@" + _clients[pos].get_hostname() + "\r\n";
		send(_clients[pos].get_socket(), reply.c_str(), reply.size(), 0);
	}

	size_t protocol::_get_client_pos_from_socket(int socket)
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
