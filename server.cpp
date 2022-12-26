#include "server.hpp"

namespace ft
{
	int const server::_buffer_size = 512;

	server::server() {}
	server::server(uint16_t port) : _port(port), _fds(), _commands_funct() {}
	server::~server() {}

	// basic socket initialization
	// int server::init_socket(void)
	// {
	// 	sockaddr_in_st socket_info;
	// 	int sockopt;

	// 	_socket = socket(AF_INET, SOCK_STREAM, 0);
	// 	// check error
	// 	sockopt = 1;
	// 	setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt));
	// 	// check error [?]
	// 	socket_info.sin_family = AF_INET;
	// 	socket_info.sin_port = htons(_port);
	// 	socket_info.sin_addr.s_addr = htonl(INADDR_ANY);
	// 	bind(_socket, (sockaddr_st*)&socket_info, sizeof(socket_info));
	// 	// check error
	// 	listen(_socket, 10);
	// 	// check error
	// 	_fds.push_back(_socket);
	// 	return 1;
	// }

	int server::init_socket(void)
	{
	    sockaddr_storage_st socket_info; // Generic socket address structure 
	    int sockopt;

	    // Create the socket
	    _socket = socket(AF_UNSPEC, SOCK_STREAM, 0);
	    if (_socket == -1)
	    {
	        perror("Error creating socket");
	        return -1;
	    }
	    // Set the SO_REUSEADDR socket option and tell the operating system that it
		// is okay for your socket to bind to a port that is already in use by
		// another socket. 
	    sockopt = 1; 
	    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt)) == -1)
	    {
	        perror("Error setting socket option");
	        return -1;
	    }
	    // Set up the sockaddr_storage_st structure
	    socket_info.ss_family = AF_INET; // or AF_INET6 for IPv6
	    if (socket_info.ss_family == AF_INET)
	    {
	        // IPv4 address
	        sockaddr_in_st* sockaddr_in = reinterpret_cast<sockaddr_in_st*>(&socket_info);
	        sockaddr_in->sin_port = htons(_port);
	        sockaddr_in->sin_addr.s_addr = htonl(INADDR_ANY);
	    }
	    else if (socket_info.ss_family == AF_INET6)
	    {
	        // IPv6 address
	        sockaddr_in6_st* sockaddr_in6 = reinterpret_cast<sockaddr_in6_st*>(&socket_info);
	        sockaddr_in6->sin6_port = htons(_port);
	        sockaddr_in6->sin6_addr = in6addr_any;
	    }
	    else
	    {
	        perror("Error: unknown address family");
	        return -1;
	    }

	    // Bind the socket to the specified address and port
	    if (bind(_socket, reinterpret_cast<sockaddr_st*>(&socket_info), sizeof(socket_info)) == -1)
	    {
	        perror("Error binding socket");
	        return -1;
	    }

	    // Put the socket into a state where it is listening for incoming connections
	    if (listen(_socket, 10) == -1)
	    {
	        perror("Error listening on socket");
	        return -1;
	    }

	    // Add the socket file descriptor to the
		_fds.push_back(_socket);
		return 0;
	}


	// // then we use select() to wait for an incoming connection
	// void server::wait_connections(void)
	// {
	// 	sockaddr_in_st client_info;
	// 	socklen_t client_info_size;
	// 	int inc_socket;
	// 	char buffer[_buffer_size];
	// 	ssize_t nb_bytes;

	// 	_init_select();
	// 	while (1)
	// 	{
	// 		_rfds_temp = _rfds;
	// 		if (select(_get_max_fd() + 1, &_rfds_temp, NULL, NULL, NULL) == -1)
	// 			continue; // = error - we continue to loop - do we handle it?
	// 		for (size_t i = 0; i < _fds.size(); i++)
	// 		{
	// 			if (FD_ISSET(_fds[i], &_rfds_temp))
	// 			{
	// 				if (_fds[i] == _socket) // if "activity" on our server socket = new incoming connection
	// 				{
	// 					client_info_size = sizeof(sockaddr_st);
	// 					// we accept the connection and add the fd to our vector
	// 					inc_socket = accept(_socket, (sockaddr_st*)&client_info, &client_info_size);
	// 					// check error
	// 					FD_SET(inc_socket, &_rfds);
	// 					_fds.push_back(inc_socket);
	// 				}
	// 				else
	// 				{
	// 					// we reset the buffer
	// 					memset(buffer, 0, _buffer_size);
	// 					nb_bytes = recv(_fds[i], buffer, _buffer_size - 1, 0);
	// 					// if (nb_bytes == -1)
	// 						// error
	// 					if (!nb_bytes) // if 0 bytes to read = client had terminated the connection
	// 					{
	// 						// close(_fds[i]); -- subject doesn't allow us to use close(), find alternative ?
	// 						// we remove the client fd from our vector
	// 						FD_CLR(_fds[i], &_rfds);
	// 						_fds.erase(_fds.begin() + i);
	// 						i--; // i can't be -1 because we will never erase the _socket fd in pos 0
	// 					}
	// 					else
	// 					{
	// 						std::string str_buffer(buffer, nb_bytes);
	// 						parse_command(buffer);

	// 						// the next lines of code were just a test to check when multiple clients
	// 						// were connected and if they were all receiving the message sent by
	// 						// another client
	// 						/*for (int j = 0; j <= _fd_max; j++)
	// 						{
	// 							if (FD_ISSET(j, &_rfds) && j != _socket && i != j)
	// 								send(j, buffer, nb_bytes, 0);
	// 						}*/
	// 					}
	// 				}
	// 			}
	// 		}
	// 	}
	// }

	void server::wait_connections(void)
	{
	    sockaddr_storage_st client_addr;
	    socklen_t addr_size = sizeof(client_addr);

	    // Set up the file descriptor set for the select function
	    fd_set read_fds;

	    // Set up the timeout for the select function
	    timeval timeout;
	    timeout.tv_sec = 5; // Wait for up to 5 seconds
	    timeout.tv_usec = 0;

	    // Enter an infinite loop to handle incoming connections
	    while (true)
	    {
	        // Set up the file descriptor set for the select function
	        FD_ZERO(&read_fds);
	        FD_SET(_socket, &read_fds);
	        for (int i = 0; i < _fds.size(); i++)
	        {
	            FD_SET(_fds[i], &read_fds);
	        }

	        // Wait for activity on any of the file descriptors
	        int result = select(_get_max_fd()+1, &read_fds, nullptr, nullptr, &timeout);
	        if (result == -1)
	        {
	            perror("Error waiting for activity on sockets");
	            continue;
	        }
	        else if (result == 0)
	        {
	            // Timeout occurred
	            continue;
	        }
	        else
	        {
	            // Activity occurred on one or more sockets
	            if (FD_ISSET(_socket, &read_fds))
	            {
	                // Accept the new connection
	                int client_fd = accept(_socket, reinterpret_cast<sockaddr_st*>(&client_addr), &addr_size);
	                if (client_fd == -1)
	                {
	                    perror("Error accepting connection");
	                    continue;
	                }

	                // Add the client file descriptor to the set of file descriptors
	                _fds.push_back(client_fd);
	            }
	            else
	            {
	                // Activity occurred on one of the client sockets
					for (int i = 0; i < _fds.size(); i++)
					{
					    int client_fd = _fds[i];
					    if (FD_ISSET(client_fd, &read_fds))
					    {
					        // Read data from the client socket
					        char buffer[_buffer_size];
					        memset(buffer, 0, _buffer_size); // Clear the buffer
					        ssize_t bytes_received = recv(client_fd, buffer, _buffer_size - 1, 0);
					        if (bytes_received == -1)
					        {
					            perror("Error reading from client socket");
					            continue;
					        }
					        else if (bytes_received == 0)
					        {
					            // The client has closed the connection
					            // Remove the client file descriptor from the set of file descriptors
					            for (std::vector<int>::iterator it = _fds.begin(); it != _fds.end(); ++it)
					            {
					                if (*it == client_fd)
					                {
					                    _fds.erase(it);
					                    break;
					                }
					            }
					            // Close the client socket
					            close(client_fd);
					        }
					        else
					        {
					            // Data was received from the client
					            // Process the data
					            std::string message(buffer, bytes_received);
					            parse_command(message);
					            // Send a reply to the client
					            reply(message);
					        }
					    }
					}

	            }
	        }
		}
	}



	void server::parse_command(std::string client_msg)
	{
		std::string line;
		size_t pos;
		std::vector<std::string> lines;

		do
		{
			// we split our string using "\r\n" as delimiter
			// if there is only 1 message in the string, no problem we just remove "\r\n"
			pos = client_msg.find("\r\n");
			line = client_msg.substr(0, pos);
			if (!line.empty())
				lines.push_back(line);
			client_msg.erase(0, pos + 2);
		} while (pos != std::string::npos);

		// just a test to check the vector
		for (size_t i = 0; i < lines.size(); i++)
			std::cout << "line = " << lines[i] << std::endl;

		// test to check message parsing and functions pointers
		for (size_t i = 0; i < lines.size(); i++)
		{
			message msg(lines[i]);
			msg.split();
			msg.print();
			reply(msg);
		}
	}

	// temporary:
	// temporary:
	void server::reply(message msg)
	{
		_commands_funct.insert(std::pair<std::string, funct>("USER", &server::test_func));
		if (_commands_funct.count(msg.get_command()))
			std::cout << (this->*_commands_funct[msg.get_command()])() << std::endl;
		// else
			// error -> send unknown command reply
	}

	// temporary:
	// temporary:
	std::string server::test_func(void)
	{
		return "this is a valid text message\n";
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
