#ifndef __PROTOCOL_HPP__
#define __PROTOCOL_HPP__

#include <map>
#include <string>
#include <vector>

#include <sys/types.h>
#include <sys/socket.h>

#include "Client.hpp"
#include "Message.hpp"

namespace ft
{
	class Protocol
	{
		private:
			typedef void (Protocol::*fncts)(Message);

		public:
			Protocol(void);
			~Protocol(void);

			void add_client(int socket);
			void delete_client(int socket);

			void parse_client_input(std::string& client_msg, int client_socket);
			void handle_message(Message msg);

			void pass_function(Message msg);
			void nick_function(Message msg);
			void user_function(Message msg);

		private:
			size_t _get_client_pos_from_socket(int socket);

			static size_t const _message_max_characters;
			static size_t const _message_max_parameters;

			Protocol(Protocol const& x);
			Protocol& operator=(Protocol const& x);

			std::map<std::string, fncts> _functions;
			std::vector<Client> _clients;
	};
}

#endif
