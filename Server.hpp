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

#include "Message.hpp"

namespace ft_irc
{

	class Server
	{
		public:
			typedef struct sockaddr sockaddr_st;
			typedef struct sockaddr_in sockaddr_in_st;
			typedef struct sockaddr_storage sockaddr_storage_st;
			typedef struct addrinfo addrinfo_st;

			Server(uint16_t port);
			int init_socket(void);
			void init_select(void);
			void wait_connections(void);
			void parse_command(std::string message);

			void reply(Message msg);
			std::string test_func(void);

		private:
			void _init_select(void);
			int _get_max_fd(void) const;

			static int const _buffer_size;
			uint16_t _port;
			int _socket;
			std::vector<int> _fds;
			fd_set _rfds;
			fd_set _rfds_temp;

			typedef std::string (Server::*funcp)(void);
			std::map<std::string, funcp> _reply_functions;
			// std::vector<Users> _users;
	};
}

#endif
