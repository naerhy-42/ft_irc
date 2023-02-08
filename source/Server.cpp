#include "Server.hpp"

namespace ft
{
	int const Server::_buffer_size = 512;

	bool Server::_server_status = true;

	Server::Server(void) : _config_status(false), _hostname("localhost"), _protocol(*this, _hostname)
	{
		time_t now = std::time(0);
		std::vector<std::string> operators;

		_creation_time = ctime(&now);
		_creation_time.erase(_creation_time.size() - 1);
		if (parse_config_file(operators))
			_config_status = true;
		_protocol.set_global_operators(operators);
		signal(SIGINT, Server::signal_handler);
	}

	Server::~Server(void) {}

	bool Server::get_config_status(void) const { return _config_status; }

	std::string const& Server::get_hostname(void) const { return _hostname; }

	std::string const& Server::get_version(void) const { return _version; }

	std::string const& Server::get_creation_time(void) const { return _creation_time; }

	int Server::get_max_fd(void) const
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

	bool Server::validate_args(std::string port, std::string password)
	{
		std::stringstream ss;

		if (password.empty() || port.empty() || port.size() > 5
				|| port.find_first_not_of("0123456789") != std::string::npos)
			return false;
		ss << port;
		ss >> _port;
		_protocol.set_password(password);
		return true;
	}

	int Server::parse_config_file(std::vector<std::string>& operators)
	{
		std::ifstream file;
		std::string line;

		file.open("config/server_conf", std::ios::in);
		if (!file.is_open())
		{
			std::cout << "The file doesn't exist, create one" << std::endl;
			return 0;
		}
		while (getline(file, line))
		{
			if (!line.empty())
			{
				std::stringstream ss(line);
				std::vector<std::string> words;
				std::string word;

				while (ss >> word)
					words.push_back(word);
				if (words.empty())
				{
					std::cout << "Config file is not valid" << std::endl;
					return 0;
				}
				if ((words[0] == "operator" && words.size() == 3))
				{
					operators.push_back(words[1]);
					operators.push_back(words[2]);
				}
				else if (words[0] == "version" && words.size() == 2)
					_version = words[1];
				else
				{
					std::cout << "Unknown line in config file, remove it" << std::endl;
					return 0;
				}
			}
		}
		file.close();
		if (operators.empty() || _version.empty())
		{
			std::cout << "Operator or version line is missing" << std::endl;
			return 0;
		}
		return 1;
	}

	int Server::init_socket(void)
	{
	    sockaddr_storage_st socket_info; // Generic socket address structure
	    int sockopt;

	    // Create the socket
	    _socket = socket(AF_INET6, SOCK_STREAM, 0);
	    if (_socket == -1)
	    {
	        // If the socket creation fails, try creating an IPv4 socket
	        _socket = socket(AF_INET, SOCK_STREAM, 0);
	        if (_socket == -1)
	        {
	            perror("Error creating socket");
	            return EXIT_FAILURE;
	        }
	    }

	    // Set the SO_REUSEADDR socket option and tell the operating system that it
	    // is okay for your socket to bind to a port that is already in use by
	    // another socket.
	    sockopt = 1;
	    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt)) == -1)
	    {
	        perror("Error setting socket option");
	        return EXIT_FAILURE;
	    }
		socklen_t addrlen = sizeof(socket_info);
		if (getsockname(_socket, reinterpret_cast<sockaddr*>(&socket_info), &addrlen) == -1)
		{
			perror("Error setting socket info");
			return EXIT_FAILURE;
		}
	    // Set up the sockaddr_storage_st structure
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
	        return EXIT_FAILURE;
	    }

	    // Bind the socket to the specified address and port
	    if (bind(_socket, reinterpret_cast<sockaddr_st*>(&socket_info), sizeof(socket_info)) == -1)
	    {
	        perror("Error binding socket");
	        return EXIT_FAILURE;
	    }

	    // Put the socket into a state where it is listening for incoming connections
	    if (listen(_socket, 10) == -1)
	    {
	        perror("Error listening on socket");
	        return EXIT_FAILURE;
	    }

	    // Add the socket file descriptor to the
		_fds.push_back(_socket);
		return EXIT_SUCCESS;
	}

	void Server::wait_connections(void)
	{
	    sockaddr_storage_st client_addr;
	    socklen_t addr_size = sizeof(client_addr);
	    fd_set read_fds;
		std::map<int, std::string> recvstr;

	    while (_server_status)
	    {
	        FD_ZERO(&read_fds);
	        FD_SET(_socket, &read_fds);
			for (std::vector<int>::const_iterator cit = _fds.begin(); cit != _fds.end(); cit++)
	            FD_SET(*cit, &read_fds);
	        int result = select(get_max_fd() + 1, &read_fds, NULL, NULL, NULL);
	        if (result == -1)
	            continue;
	        else
	        {
	            if (FD_ISSET(_socket, &read_fds))
	            {
	                int client_fd = accept(_socket, reinterpret_cast<sockaddr_st*>(&client_addr), &addr_size);
	                if (client_fd == -1)
	                {
						sleep(3);
	                    continue;
	                }
	                _fds.push_back(client_fd);
					_protocol.add_client(client_fd);
	            }
	            else
	            {
					for (std::vector<int>::size_type i = 0; i < _fds.size(); i++)
					{
					    int client_fd = _fds[i];
					    if (FD_ISSET(client_fd, &read_fds))
					    {
					        char buffer[_buffer_size];
					        memset(buffer, 0, _buffer_size);
					        ssize_t bytes_received = recv(client_fd, buffer, _buffer_size - 1, 0);
					        if (!bytes_received || (bytes_received == -1 && errno == EWOULDBLOCK))
								close_socket_connection(client_fd);
					        else
					        {
								std::string str_buffer(buffer, bytes_received);

								if (recvstr.count(client_fd))
									recvstr.find(client_fd)->second += str_buffer;
								else
									recvstr.insert(std::pair<int, std::string>(client_fd, str_buffer));
					        }
					    }
					}
					check_received_input(recvstr);
	            }
	        }
		}
	}

	void Server::check_received_input(std::map<int, std::string>& recvstr)
	{
		std::map<int, std::string>::iterator it;
		std::vector<std::map<int, std::string>::iterator> str_to_delete;
		std::vector<std::map<int, std::string>::iterator>::iterator itt;

		for (it = recvstr.begin(); it != recvstr.end(); it++)
		{
			if (it->second.find_last_of("\r\n") == it->second.size() - 1)
			{
				_protocol.parse_client_input(it->first, it->second);
				str_to_delete.push_back(it);
			}
		}
		for (itt = str_to_delete.begin(); itt != str_to_delete.end(); itt++)
			recvstr.erase(*itt);
	}

	void Server::close_socket_connection(int socket)
	{
		_protocol.remove_client_from_channels(_protocol.get_client_from_socket(socket));
		for (std::vector<int>::iterator it = _fds.begin(); it != _fds.end(); ++it)
		{
			if (*it == socket)
			{
				_protocol.delete_client(*it);
				_fds.erase(it);
				break;
			}
		}
		close(socket);
	}

	void Server::set_server_status(bool status) { _server_status = status; }

	void Server::signal_handler(int signum)
	{
		std::cout << "- signal has been caught, closing server (" << signum << ") -" << std::endl;
		_server_status = false;
	}
}
