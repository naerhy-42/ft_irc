#include "message.hpp"

namespace ft
{
	int const message::_max_chars = 512; // message from client cannot hold more than 512 chars
	int const message::_max_params = 15; // message from client cannot hold more than 15 parameters

	message::message(std::string const& base_message) : _base_message(base_message)
	{
		// add message validation
		// if message > 512 || > 15 paremeters = error
	}

	void message::split(void)
	{
		std::string remainder;
		size_t remainder_pos;
		std::stringstream ss;
		std::string word;

		// find the first occurrence of " :" = this is the remainder of a message
		// if it finds it -> extract if from base_message and store it
		remainder_pos = _base_message.find(" :");
		if (remainder_pos != std::string::npos)
		{
			remainder.append(_base_message, remainder_pos + 2, std::string::npos);
			_base_message.erase(remainder_pos, std::string::npos);
			_remainder = remainder;
		}
		// to use getline, we have to create a stringstream from our string
		ss.str(_base_message);
		// now we split our message:
			// - if word begins by ":" -> it is the sender of the message
			// - the first word we check is necessarily the command, otherwise it's an argument
		// potential issue: if message contains multiple spaces chained together -> bad parsing
		// but it should never be a case as irc clients only send "valid" messages
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

	void message::print(void) const
	{
		std::cout << "origin = " << _origin << std::endl;
		std::cout << "command = " << _command << std::endl;
		for (size_t i = 0; i < _parameters.size(); i++)
			std::cout << "parameter = " << _parameters[i] << std::endl;
		std::cout << "remainder = " << _remainder << std::endl;
	}

	std::string const& message::get_origin(void) const { return _origin; }

	std::string const& message::get_command(void) const { return _command; }

	std::vector<std::string> const& message::get_parameters(void) const { return _parameters; }

	std::string const& message::get_remainder(void) const { return _remainder; }
}
