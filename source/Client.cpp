#include "Client.hpp"


namespace ft
{
	Client::Client(int socket) : _socket(socket), _nickname("*"), _password_status(false),
			_nickname_status(false), _registration_status(false) {}

	Client::~Client(void) {}

	int Client::get_socket(void) const { return _socket; }

	std::string const& Client::get_nickname(void) const { return _nickname; }

	std::string const& Client::get_username(void) const { return _username; }

	std::string const& Client::get_hostname(void) const { return _hostname; }

	std::string const& Client::get_servername(void) const { return _servername; }

	std::string const& Client::get_real_name(void) const { return _real_name; }

	std::string Client::get_prefix(void) const
	{
		return _nickname + "!" + _username + "@" + _hostname;
	}

	bool Client::get_password_status(void) const { return _password_status; }

	bool Client::get_nickname_status(void) const { return _nickname_status; }

	bool Client::get_registration_status(void) const { return _registration_status; }

	Modes& Client::get_modes_obj(void) { return _modes; }
	Modes const& Client::get_modes_obj(void) const { return _modes; }

	bool Client::is_global_operator(void) const { return get_modes_obj().has_mode('o'); }

	bool Client::is_invisible(void) const { return get_modes_obj().has_mode('i'); }

	void Client::set_nickname(std::string const &nickname) { _nickname = nickname; }

	void Client::set_username(std::string const &username) { _username = username; }

	void Client::set_hostname(std::string const &hostname) { _hostname = hostname; }

	void Client::set_servername(std::string const &servername) { _servername = servername; }

	void Client::set_real_name(std::string const &real_name) { _real_name = real_name; }

	void Client::set_password_status(bool status) { _password_status = status; }

	void Client::set_nickname_status(bool status) { _nickname_status = status; }

	void Client::set_registration_status(bool status) { _registration_status = status; }
}
