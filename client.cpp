#include "client.hpp"

namespace ft
{
	client::client(int socket) : _socket(socket), _registration_status(false) {}

	int client::get_socket(void) const { return _socket; }

	std::string const& client::get_nickname(void) const { return _nickname; }

	std::string const& client::get_username(void) const { return _username; }

	std::string const& client::get_hostname(void) const { return _hostname; }

	std::string const& client::get_real_name(void) const { return _real_name; }

	bool client::get_registration_status(void) const { return _registration_status; }

	void client::set_nickname(std::string const& nickname) { _nickname = nickname; }

	void client::set_username(std::string const& username) { _username = username; }

	void client::set_hostname(std::string const& hostname) { _hostname = hostname; }

	void client::set_real_name(std::string const& real_name) { _real_name = real_name; }

	void client::set_registration_status(bool status) { _registration_status = status; }
}
