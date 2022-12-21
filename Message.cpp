#include "Message.hpp"

namespace ft_irc
{
	int const Message::_max_chars = 512;
	int const Message::_max_params = 15;

	Message::Message(std::string const& base_message) : _base_message(base_message)
	{
		// check if nb_chars is not > max
	}

	void Message::split(void)
	{
		std::string remainder;
		size_t remainder_pos;
		std::stringstream ss;
		std::string word;

		remainder_pos = _base_message.find(" :");
		if (remainder_pos != std::string::npos)
		{
			remainder.append(_base_message, remainder_pos + 2, std::string::npos);
			_base_message.erase(remainder_pos, std::string::npos);
			_remainder = remainder;
		}
		ss.str(_base_message);
		while (getline(ss, word, ' ')) // potential issue: store " " as string if multiple spaces chained
		{
			if (word[0] == ':')
				_origin = word;
			else
			{
				if (_command.empty())
					_command = word;
				else
					_command_params.push_back(word);
			}
		}
	}

	void Message::print(void) const
	{
		std::cout << "origin = " << _origin << std::endl;
		std::cout << "command = " << _command << std::endl;
		for (size_t i = 0; i < _command_params.size(); i++)
			std::cout << "parameter = " << _command_params[i] << std::endl;
		std::cout << "remainder = " << _remainder << std::endl;
	}
	std::string const& Message::get_command(void) const { return _command; }
}
