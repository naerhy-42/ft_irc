#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <iostream>

#include <cstdio>
#include <cstdlib>

#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <sstream>
#include <vector>
#include <map>
#include <utility>

#include "message.hpp"
#include "protocol.hpp"

namespace ft
{

	class server
	{
		private:
			// using typedef in order to avoid typing struct + type
			typedef struct sockaddr sockaddr_st;
			typedef struct sockaddr_in sockaddr_in_st;
			typedef struct sockaddr_storage sockaddr_storage_st;
			typedef struct addrinfo addrinfo_st;

		public:
			server(void);
			// server(server const& x);
			// server& operator=(server const& x);
			// ~server(void);

			bool validate_args(std::string port, std::string password);
			int init_socket(void);
			void init_select(void);
			void wait_connections(void);
			void parse_command(std::string message);

		private:
			void _init_select(void);
			int _get_max_fd(void) const;

			static int const _buffer_size;
			uint16_t _port;
			std::string _password;
			int _socket;
			std::vector<int> _fds;
			fd_set _rfds;
			fd_set _rfds_temp;
			protocol _protocol;

			// std::vector<user> _users; -- to hold every users informations
			// std::vector<channel> _channels; -- to hold every channels informations
	};
}

#endif
