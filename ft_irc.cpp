#include "Server.hpp"

// socket() => bind() => listen() => select() => accept()

int main(int argc, char** argv)
{
	if (argc != 3)
		return EXIT_FAILURE;
	// check if args are valid
	Server server(8080);
	if (!server.init_socket())
		return EXIT_FAILURE;
	server.init_select();
	server.wait_connections();
	return EXIT_SUCCESS;
}
