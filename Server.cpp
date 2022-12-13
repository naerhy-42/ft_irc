#include "Server.hpp"

Server::Server(uint16_t port) : _port(port) {}

int Server::init_socket(void)
{
	sockaddr_in_st socket_info;
	int sockopt;

	_socket = socket(AF_INET, SOCK_STREAM, 0);
	// check error
	sockopt = 1;
	setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt));
	// check error [?]
	socket_info.sin_family = AF_INET;
	socket_info.sin_port = htons(_port);
	socket_info.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(_socket, (sockaddr_st*)&socket_info, sizeof(socket_info));
	// check error
	listen(_socket, 10);
	// check error
	return 1;
}

void Server::init_select(void)
{
	FD_ZERO(&_rfds);
	FD_ZERO(&_rfds_temp);
	FD_SET(_socket, &_rfds);
	_fd_max = _socket;
}

void Server::wait_connections(void)
{
	sockaddr_in_st client_info;
	socklen_t client_info_size;
	int inc_socket;
	char buffer[BUFFER_LENGTH];
	ssize_t nb_bytes;
	
	while (1)
	{
		_rfds_temp = _rfds;
		if (select(_fd_max + 1, &_rfds_temp, NULL, NULL, NULL) == -1)
			continue; // = error
		for (int i = 0; i <= _fd_max; i++)
		{
			if (FD_ISSET(i, &_rfds_temp))
			{
				if (i == _socket)
				{
					client_info_size = sizeof(sockaddr_st);
					inc_socket = accept(_socket, (sockaddr_st*)&client_info, &client_info_size);
					// check error
					FD_SET(inc_socket, &_rfds);
					if (inc_socket > _fd_max)
						_fd_max = inc_socket;
				}
				else
				{
					memset(buffer, 0, BUFFER_LENGTH);
					nb_bytes = recv(i, buffer, BUFFER_LENGTH - 1, 0);
					if (nb_bytes <= 0) // = error
						FD_CLR(i, &_rfds);
					else
					{
						for (int j = 0; j <= _fd_max; j++)
						{
							if (FD_ISSET(j, &_rfds) && j != _socket && i != j)
								send(j, buffer, nb_bytes, 0);
						}
					}
				}
			}
		}
	}
}
