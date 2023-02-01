#include "../include/ClientMessage.hpp"

namespace ft
{
	ClientMessage::ClientMessage(int socket, std::string& message) : _socket(socket)
	{
		std::string remainder;
		size_t remainder_pos = message.find(" :");
		std::stringstream ss;
		std::string word;

		if (remainder_pos != std::string::npos)
		{
			remainder.append(message, remainder_pos + 2, std::string::npos);
			message.erase(remainder_pos, std::string::npos);
			_remainder = remainder;
		}
		ss.str(message);
		while (ss >> word)
		{
			if (_command.empty())
				_command = word;
			else
				_parameters.push_back(word);
		}
	}

	ClientMessage::~ClientMessage(void) {}

	int ClientMessage::get_socket(void) const { return _socket; }

	std::string const& ClientMessage::get_command(void) const { return _command; }

	std::vector<std::string> const& ClientMessage::get_parameters(void) const
	{
		return _parameters;
	}

	std::string const& ClientMessage::get_remainder(void) const { return _remainder; }
}
