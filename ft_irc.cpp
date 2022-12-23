#include "server.hpp"

int main(int argc, char** argv)
{
	ft::server server;

	if (argc != 3 || !server.validate_args(*(argv + 1), *(argv + 2)))
	{
		std::cout << "Error: you have to enter a valid port and password" << std::endl;
		return EXIT_FAILURE;
	}
	server.init_socket();
	if (!server.init_socket())
		return EXIT_FAILURE;
	server.wait_connections();
	return EXIT_SUCCESS;
}
