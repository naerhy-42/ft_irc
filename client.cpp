#include "client.hpp"

namespace ft
{
	client::client(int socket) : _socket(socket) {}

	int client::get_socket(void) const { return _socket; }

	std::string const& client::get_nickname(void) const { return _nickname; }

	void client::set_nickname(std::string const& nickname) { _nickname = nickname; }
}
