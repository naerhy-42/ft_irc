#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <cstdio>
#include <cstdlib>

#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUFFER_LENGTH 2048

typedef struct sockaddr sockaddr_st;
typedef struct sockaddr_in sockaddr_in_st;

class Server
{
	public:
		Server(uint16_t port);
		int init_socket(void);
		void init_select(void);
		void wait_connections(void);

	private:
		uint16_t _port;
		int _socket;

		fd_set _rfds;
		fd_set _rfds_temp;
		int _fd_max;
};

#endif
