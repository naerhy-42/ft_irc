#include "../include/ClientMessage.hpp"

namespace ft
{
	ClientMessage::ClientMessage(std::string& base_message, int socket) : _socket(socket)
	{
		std::string remainder;
		size_t remainder_pos = base_message.find(" :");
		std::stringstream ss;
		std::string word;

		if (remainder_pos != std::string::npos)
		{
			remainder.append(base_message, remainder_pos + 2, std::string::npos);
			base_message.erase(remainder_pos, std::string::npos);
			_remainder = remainder;
		}
		ss.str(base_message);
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

	std::string const& ClientMessage::get_origin(void) const { return _origin; }

	std::string const& ClientMessage::get_command(void) const { return _command; }

	std::vector<std::string> const& ClientMessage::get_parameters(void) const
	{
		return _parameters;
	}

	std::string const& ClientMessage::get_remainder(void) const { return _remainder; }
}
