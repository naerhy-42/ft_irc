#ifndef __CLIENT_MESSAGE_HPP__
#define __CLIENT_MESSAGE_HPP__

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Client.hpp"

namespace ft
{
	class ClientMessage
	{
		public:
			ClientMessage(Client* client, std::string& message);
			~ClientMessage(void);

			Client* get_client(void) const;
			std::string const& get_command(void) const;
			std::vector<std::string> const& get_parameters(void) const;
			std::string const& get_remainder(void) const;

		private:
			// message(message const& x);
			// message& operator=(message const& x);

			Client* _client;
			std::string _command;
			std::vector<std::string> _parameters;
			std::string _remainder;
	};
}

#endif
