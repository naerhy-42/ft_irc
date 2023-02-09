#include "Bot.hpp"

namespace ft
{
	Bot::Bot(void) : _name("Cyber42"), _endl("\r\n") {}

	Bot::~Bot(void) { close(_socket); }

	int Bot::get_socket(void) const { return _socket; }

	int Bot::init(uint16_t port)
	{
		struct sockaddr_in x;

		_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (_socket == -1)
			return 0;
		x.sin_family = AF_INET;
		x.sin_port = htons(port);
		x.sin_addr.s_addr = htonl(INADDR_ANY);
		if (connect(_socket, (struct sockaddr*)&x, sizeof(x)) == -1)
		{
			close(_socket);
			return 0;
		}
		return 1;
	}

	void Bot::connect_to_server(std::string const& password) const
	{
		std::string pass_message = "PASS " + password + _endl;
		std::string nick_message = "NICK " + _name + _endl;
		std::string user_message = "USER " + _name + " " + _name + " localhost :" + _name + _endl;
		std::string send_message = pass_message + nick_message + user_message;

		send(_socket, send_message.c_str(), send_message.size(), 0);
	}

	void Bot::get_operator_privileges(void) const
	{
		std::string oper_message = "OPER " + _name + " " + _name + _endl;

		send(_socket, oper_message.c_str(), oper_message.size(), 0);
	}

	void Bot::parse_recv(std::string message) const
	{
		std::vector<std::string> words;
		std::stringstream ss(message);
		std::string word;

		while (ss >> word)
			words.push_back(word);
		if (words[1] == "INVITE")
		{
			std::string message = "JOIN " + words[3] + "\r\n";

			std::cout << "Bot has received an invite" << std::endl;
			send(_socket, message.c_str(), message.size(), 0);
		}
		else if (words[1] == "PRIVMSG")
		{
			size_t sentence_pos = message.find(" :");
			if (message.find("epitech", sentence_pos + 2) != std::string::npos)
			{
				std::string target = message.substr(1, message.find('!') - 1);
				std::string kill_message = "KILL " + target + " :don't type the forbidden word" + _endl;
				send(_socket, kill_message.c_str(), kill_message.size(), 0);
			}
		}
		else { /* no action is needed */ }
	}
}
