#include "../include/Client.hpp"

namespace ft
{
	Client::Client(int socket) : _socket(socket), _nickname("*"), _nickname_status(false),
			_registration_status(false), _password_status(false), _global_operator_status(false) {}

	int Client::get_socket(void) const { return _socket; }

	std::string const &Client::get_nickname(void) const { return _nickname; }

	std::string const &Client::get_username(void) const { return _username; }

	std::string const &Client::get_hostname(void) const { return _hostname; }

	std::string const &Client::get_servername(void) const { return _servername; }

	std::string const &Client::get_real_name(void) const { return _real_name; }

	bool Client::get_nickname_status(void) const { return _nickname_status; }

	bool Client::get_registration_status(void) const { return _registration_status; }

	bool Client::get_password_status(void) const { return _password_status; }

	bool Client::get_global_operator_status(void) const { return _global_operator_status; }

	void Client::set_nickname(std::string const &nickname) { _nickname = nickname; }

	void Client::set_username(std::string const &username) { _username = username; }

	void Client::set_hostname(std::string const &hostname) { _hostname = hostname; }

	void Client::set_servername(std::string const &servername) { _servername = servername; }

	void Client::set_real_name(std::string const &real_name) { _real_name = real_name; }

	void Client::set_nickname_status(bool status) { _nickname_status = status; }

	void Client::set_registration_status(bool status) { _registration_status = status; }

	void Client::set_password_status(bool status) { _password_status = status; }

	void Client::set_global_operator_status(bool status) { _global_operator_status = status; }

	Client &Client::operator=(Client const &other)
	{
		if (this != &other)
		{
			_socket = other._socket;
			_nickname = other._nickname;
			_username = other._username;
			_hostname = other._hostname;
			_servername = other._servername;
			_real_name = other._real_name;
			_nickname_status = other._nickname_status;
			_registration_status = other._registration_status;
			_password_status = other._password_status;
		}
		return *this;
	}

	bool operator==(Client const &lhs, Client const &rhs)
	{
    	return lhs.get_socket() == rhs.get_socket()
        	&& lhs.get_nickname() == rhs.get_nickname()
        	&& lhs.get_username() == rhs.get_username()
        	&& lhs.get_hostname() == rhs.get_hostname()
        	&& lhs.get_servername() == rhs.get_servername()
        	&& lhs.get_real_name() == rhs.get_real_name()
			&& lhs.get_nickname_status() == rhs.get_nickname_status()
        	&& lhs.get_registration_status() == rhs.get_registration_status()
        	&& lhs.get_password_status() == rhs.get_password_status();
	}
}
