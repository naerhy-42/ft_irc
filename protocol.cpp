#include "protocol.hpp"

// remove after test
// remove after test
// remove after test
// remove after test
#include <iostream>

namespace ft
{
	protocol::protocol(void) : _functions()
	{
		_functions.insert(std::pair<std::string, fncts>("NICK", &protocol::nick_function));
	}

	protocol::~protocol(void) {}

	void protocol::handle_message(message msg)
	{
		if (_functions.count(msg.get_command()))
			(this->*_functions[msg.get_command()])();
		// else do something if command is unknown??
	}

	std::string protocol::nick_function(void)
	{
		// check if parameter in message in not empty
		// check if nickname contains only valid characters
		// check if nickname is free
		std::cout << "nick function has been called" << std::endl;
		return "";
	}
}
