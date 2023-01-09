#ifndef __PROTOCOL_HPP__
#define __PROTOCOL_HPP__

#include <map>
#include <string>
#include <vector>

#include <sys/types.h>
#include <sys/socket.h>

#include "Client.hpp"
#include "Message.hpp"
#include "Reply.hpp"

namespace ft
{
	class Server;

	class Protocol
	{
		private:
			typedef void (Protocol::*fncts)(Message);

		public:
			Protocol(Server const* server);
			~Protocol(void);

			void set_password(std::string const& password);

			void add_client(int socket);
			void delete_client(int socket);

			void parse_client_input(std::string& client_msg, int client_socket);
			void handle_message(Message msg);

			void pass_function(Message msg);
			void nick_function(Message msg);
			void user_function(Message msg);

		private:
			Client& _get_client_from_socket(int socket);

			static size_t const _message_max_characters;
			static size_t const _message_max_parameters;

			Protocol(Protocol const& x);
			Protocol& operator=(Protocol const& x);

			Server const* _server;
			std::string _password;
			std::map<std::string, fncts> _functions;
			std::vector<Client> _clients;
	};
}

#endif
