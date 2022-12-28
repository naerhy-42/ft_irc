#include "server.hpp"

namespace ft
{
	int const server::_buffer_size = 512;

	server::server(void) : _fds(), _protocol() {}

	bool server::validate_args(std::string port, std::string password)
	{
		std::stringstream ss;

		if (password.empty() || port.empty() || port.size() > 5
				|| port.find_first_not_of("0123456789") != std::string::npos)
			return false;
		ss << port;
		ss >> _port;
		_password = password;
		return true;
	}

	// basic socket initialization
	int server::init_socket(void)
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

	// then we use select() to wait for an incoming connection
	void server::wait_connections(void)
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
				continue; // = error - we continue to loop - do we handle it?
			for (size_t i = 0; i < _fds.size(); i++)
			{
				if (FD_ISSET(_fds[i], &_rfds_temp))
				{
					if (_fds[i] == _socket) // if "activity" on our server socket = new incoming connection
					{
						client_info_size = sizeof(sockaddr_st);
						// we accept the connection and add the fd to our vector
						inc_socket = accept(_socket, (sockaddr_st*)&client_info, &client_info_size);
						// check error
						FD_SET(inc_socket, &_rfds);
						_fds.push_back(inc_socket);
						// protocol => create new client with fd as parameter
						// _clients.push_back(client(inc_socket));
					}
					else
					{
						// we reset the buffer
						memset(buffer, 0, _buffer_size);
						nb_bytes = recv(_fds[i], buffer, _buffer_size - 1, 0);
						// if (nb_bytes == -1)
							// error
						if (!nb_bytes) // if 0 bytes to read = client had terminated the connection
						{
							// close(_fds[i]); -- subject doesn't allow us to use close(), find alternative ?
							// we remove the client fd from our vector
							FD_CLR(_fds[i], &_rfds);
							_fds.erase(_fds.begin() + i);
							// call protocol to remove client
							i--; // i can't be -1 because we will never erase the _socket fd in pos 0
						}
						else
						{
							std::string str_buffer(buffer, nb_bytes);
							_protocol.parse_client_input(str_buffer, _fds[i]);
						}
					}
				}
			}
		}
	}

	void server::_init_select(void)
	{
		FD_ZERO(&_rfds);
		FD_ZERO(&_rfds_temp);
		FD_SET(_socket, &_rfds);
	}

	int server::_get_max_fd(void) const
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
