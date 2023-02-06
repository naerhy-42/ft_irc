#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <iostream>

#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <sstream>
#include <vector>
#include <map>
#include <utility>

#include "Client.hpp"
#include "Protocol.hpp"

namespace ft
{
	class Server
	{
		private:
			// using typedef in order to avoid typing struct + type
			typedef struct sockaddr sockaddr_st;
			typedef struct sockaddr_in sockaddr_in_st;
			typedef struct sockaddr_in6 sockaddr_in6_st;
			typedef struct sockaddr_storage sockaddr_storage_st;
			typedef struct addrinfo addrinfo_st;

		public:
			Server(void);
			~Server(void);

			bool get_config_status(void) const;
			std::string const& get_hostname(void) const;
			std::string const& get_version(void) const;
			std::string const& get_creation_time(void) const;
			int get_max_fd(void) const;

			bool validate_args(std::string port, std::string password);
			int parse_config_file(std::vector<std::string>& op_lines);

			int init_socket(void);
			void wait_connections(void);
			void close_socket_connection(int socket);

		private:
			Server(Server const& x);
			Server& operator=(Server const& x);

			static int const _buffer_size;
			uint16_t _port;
			int _socket;
			bool _config_status;
			std::vector<int> _fds;
			fd_set _rfds;
			std::string _hostname;
			std::string _version;
			std::string _creation_time;
			Protocol _protocol;
	};
}

#endif
