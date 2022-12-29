#ifndef __PROTOCOL_HPP__
#define __PROTOCOL_HPP__

#include <map>
#include <string>
#include <vector>

#include <sys/types.h>
#include <sys/socket.h>

#include "client.hpp"
#include "message.hpp"

namespace ft
{
	class protocol
	{
		private:
			typedef void (protocol::*fncts)(message);

		public:
			protocol(void);
			~protocol(void);

			void add_client(int socket);
			void delete_client(int socket);

			void parse_client_input(std::string& client_msg, int client_socket);
			void handle_message(message msg);

			void nick_function(message msg);
			void user_function(message msg);

		private:
			size_t _get_client_pos_from_socket(int socket);

			protocol(protocol const& x);
			protocol& operator=(protocol const& x);

			std::map<std::string, fncts> _functions;
			std::vector<client> _clients;
	};
}

#endif
