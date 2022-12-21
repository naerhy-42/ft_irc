Server::#include "Server.hpp"

// socket() => bind() => listen() => select() => accept()

int main(int argc, char** argv)
{
	if (argc != 3)
		return EXIT_FAILURE;
	// check if args are valid
	ft_irc::Server server(8080);
	if (!server.init_socket())
		return EXIT_FAILURE;
	server.wait_connections();
	return EXIT_SUCCESS;
}
