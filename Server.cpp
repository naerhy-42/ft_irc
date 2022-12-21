#include "Server.hpp"

namespace ft_irc
{
	int const Server::_buffer_size = 512;

	Server::Server(uint16_t port) : _port(port), _fds(), _reply_functions() {}

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
		_fds.push_back(_socket);
		return 1;
	}

	void Server::wait_connections(void)
	{
		sockaddr_in_st client_info;
		socklen_t client_info_size;
		int inc_socket;
		char buffer[_buffer_size];
		ssize_t nb_bytes;

		_init_select();
		while (1)
		{
			_rfds_temp = _rfds;
			if (select(_get_max_fd() + 1, &_rfds_temp, NULL, NULL, NULL) == -1)
				continue; // = error - we continue to loop
			for (size_t i = 0; i < _fds.size(); i++)
			{
				if (FD_ISSET(_fds[i], &_rfds_temp))
				{
					if (_fds[i] == _socket)
					{
						client_info_size = sizeof(sockaddr_st);
						inc_socket = accept(_socket, (sockaddr_st*)&client_info, &client_info_size);
						// check error
						FD_SET(inc_socket, &_rfds);
						_fds.push_back(inc_socket);
					}
					else
					{
						memset(buffer, 0, _buffer_size);
						nb_bytes = recv(_fds[i], buffer, _buffer_size - 1, 0);
						// if (nb_bytes == -1)
							// error
						if (!nb_bytes) // = connection ended -> closing socket
						{
							// close(_fds[i]);
							FD_CLR(_fds[i], &_rfds);
							_fds.erase(_fds.begin() + i);
							i--; // i can't be -1 because we will never erase the _socket fd in pos 0
						}
						else
						{
							std::string str_buffer(buffer, nb_bytes);
							parse_command(buffer);
							/*for (int j = 0; j <= _fd_max; j++)
							{
								if (FD_ISSET(j, &_rfds) && j != _socket && i != j)
									send(j, buffer, nb_bytes, 0);
							}*/
						}
					}
				}
			}
		}
	}

	void Server::parse_command(std::string message)
	{
		std::string line;
		size_t pos;
		std::vector<std::string> lines;

		do
		{
			pos = message.find("\r\n");
			line = message.substr(0, pos);
			if (!line.empty())
				lines.push_back(line);
			message.erase(0, pos + 2);
		}
		while (pos != std::string::npos);

		for (size_t i = 0; i < lines.size(); i++)
			std::cout << "line = " << lines[i] << std::endl;

		for (size_t i = 0; i < lines.size(); i++)
		{
			Message msg(lines[i]);
			msg.split();
			// msg.print();
			reply(msg);
		}
	}

	// temporary:
	// temporary:
	void Server::reply(Message msg)
	{
		_reply_functions.insert(std::pair<std::string, funcp>("USER", &Server::test_func));
		if (_reply_functions.count(msg.get_command()))
			std::cout << (this->*_reply_functions[msg.get_command()])() << std::endl;
		// else
			// error -> send unknown command reply
	}

	// temporary:
	// temporary:
	std::string Server::test_func(void)
	{
		return "this is a valid text message\n";
	}

	void Server::_init_select(void)
	{
		FD_ZERO(&_rfds);
		FD_ZERO(&_rfds_temp);
		FD_SET(_socket, &_rfds);
	}

	int Server::_get_max_fd(void) const
	{
		int max_fd;

		max_fd = _socket;
		for (std::vector<int>::const_iterator cit = _fds.begin(); cit != _fds.end(); cit++)
		{
			if (*cit > max_fd)
				max_fd = *cit;
		}
		return max_fd;
	}
}
