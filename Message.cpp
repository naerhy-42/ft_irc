#include "Message.hpp"

namespace ft
{
	Message::Message(std::string& base_message, int socket) : _socket(socket)
	{
		std::string remainder;
		size_t remainder_pos;
		std::stringstream ss;
		std::string word;

		remainder_pos = base_message.find(" :");
		if (remainder_pos != std::string::npos)
		{
			remainder.append(base_message, remainder_pos + 2, std::string::npos);
			base_message.erase(remainder_pos, std::string::npos);
			_remainder = remainder;
		}
		ss.str(base_message);
		while (getline(ss, word, ' '))
		{
			if (word[0] == ':')
				_origin = word;
			else
			{
				if (_command.empty())
					_command = word;
				else
					_parameters.push_back(word);
			}
		}
	}

	int Message::get_socket(void) const { return _socket; }

	std::string const& Message::get_origin(void) const { return _origin; }

	std::string const& Message::get_command(void) const { return _command; }

	std::vector<std::string> const& Message::get_parameters(void) const { return _parameters; }

	std::string const& Message::get_remainder(void) const { return _remainder; }
}
