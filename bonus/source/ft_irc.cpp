#include "Server.hpp"

int main(int argc, char** argv)
{
	ft::Server server;

	if (argc != 3 || !server.validate_args(*(argv + 1), *(argv + 2)))
	{
		std::cout << "Error: you have to enter a valid port and password" << std::endl;
		return EXIT_FAILURE;
	}
	if (!server.get_config_status() || server.init_socket() == EXIT_FAILURE || !server.init_bot())
		return EXIT_FAILURE;
	server.wait_connections();
	return EXIT_SUCCESS;
}
