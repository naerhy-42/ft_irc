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

	bool Client::get_password_status(void) const { return _password_status; }

	bool Client::get_nickname_status(void) const { return _nickname_status; }

	bool Client::get_registration_status(void) const { return _registration_status; }

	bool Client::has_mode(char mode) const
	{
		std::vector<char>::const_iterator cit;

		for (cit = _modes.begin(); cit != _modes.end(); cit++)
		{
			if (*cit == mode)
				return true;
		}
		return false;
	}

	std::vector<char> const& Client::get_modes(void) const { return _modes; }

	std::string Client::get_modes_str(void) const
	{
		std::string modes_str;
		std::vector<char>::const_iterator cit;

		for (cit = _modes.begin(); cit != _modes.end(); cit++)
			modes_str.append(1, *cit);
		return modes_str;
	}

	void Client::set_nickname(std::string const &nickname) { _nickname = nickname; }

	void Client::set_username(std::string const &username) { _username = username; }

	void Client::set_hostname(std::string const &hostname) { _hostname = hostname; }

	void Client::set_servername(std::string const &servername) { _servername = servername; }

	void Client::set_real_name(std::string const &real_name) { _real_name = real_name; }

	void Client::set_password_status(bool status) { _password_status = status; }

	void Client::set_nickname_status(bool status) { _nickname_status = status; }

	void Client::set_registration_status(bool status) { _registration_status = status; }

	void Client::set_mode(char sign, char mode)
	{
		if (sign == '+')
			_modes.push_back(mode);
		else
		{
			std::vector<char>::iterator it;

			for (it = _modes.begin(); it != _modes.end(); it++)
			{
				if (*it == mode)
				{
					_modes.erase(it);
					break;
				}
			}
		}
	}
}
