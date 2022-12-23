#include "server.hpp"

int main(int argc, char** argv)
{
	if (argc != 3)
		return EXIT_FAILURE;
	// check if args are valid
	ft::server server(8080);
	if (!server.init_socket())
		return EXIT_FAILURE;
	server.wait_connections();
	return EXIT_SUCCESS;
}
