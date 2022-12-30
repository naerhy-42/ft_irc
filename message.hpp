#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__

#include <sstream>
#include <string>
#include <vector>

#include <iostream>

namespace ft
{
	// might delete origin -> useless if messages are only sent by clients
	class message
	{
		public:
			message(std::string& base_message, int socket);
			// message(message const& x);
			// message& operator=(message const& x);
			// ~message(void);

			int get_socket(void) const;
			std::string const& get_origin(void) const;
			std::string const& get_command(void) const;
			std::vector<std::string> const& get_parameters(void) const;
			std::string const& get_remainder(void) const;

		private:
			int _socket;
			std::string _origin;
			std::string _command;
			std::vector<std::string> _parameters;
			std::string _remainder;
	};
}

#endif
