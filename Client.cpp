#include "Client.hpp"

namespace ft
{
	Client::Client(int socket) : _socket(socket), _registration_status(false) {}

	int Client::get_socket(void) const { return _socket; }

	std::string const& Client::get_nickname(void) const { return _nickname; }

	std::string const& Client::get_username(void) const { return _username; }

	std::string const& Client::get_hostname(void) const { return _hostname; }

	std::string const& Client::get_real_name(void) const { return _real_name; }

	bool Client::get_registration_status(void) const { return _registration_status; }

	void Client::set_nickname(std::string const& nickname) { _nickname = nickname; }

	void Client::set_username(std::string const& username) { _username = username; }

	void Client::set_hostname(std::string const& hostname) { _hostname = hostname; }

	void Client::set_real_name(std::string const& real_name) { _real_name = real_name; }

	void Client::set_registration_status(bool status) { _registration_status = status; }
}
