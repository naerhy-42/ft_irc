#ifndef __CLIENT_MESSAGE_HPP__
#define __CLIENT_MESSAGE_HPP__

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace ft
{
	class ClientMessage
	{
		public:
			ClientMessage(std::string& base_message, int socket);
			~ClientMessage(void);

			int get_socket(void) const;
			std::string const& get_origin(void) const;
			std::string const& get_command(void) const;
			std::vector<std::string> const& get_parameters(void) const;
			std::string const& get_remainder(void) const;

		private:
			// message(message const& x);
			// message& operator=(message const& x);

			int _socket;
			std::string _command;
			std::vector<std::string> _parameters;
			std::string _remainder;
	};
}

#endif
