#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include <iostream>
#include <string>
#include <vector>

#include "Modes.hpp"

namespace ft
{
	class Client
	{
	public:
		Client(int socket);
		~Client(void);

		int get_socket(void) const;
		std::string const& get_nickname(void) const;
		std::string const& get_username(void) const;
		std::string const& get_hostname(void) const;
		std::string const& get_servername(void) const;
		std::string const& get_real_name(void) const;
		std::string get_prefix(void) const;
		bool get_password_status(void) const;
		bool get_nickname_status(void) const;
		bool get_registration_status(void) const;
		Modes& get_modes_obj(void);
		Modes const& get_modes_obj(void) const;

		bool is_global_operator(void) const;
		bool is_invisible(void) const;

		void set_nickname(std::string const &nickname);
		void set_username(std::string const &username);
		void set_hostname(std::string const &hostname);
		void set_servername(std::string const &servername);
		void set_real_name(std::string const &real_name);
		void set_password_status(bool status);
		void set_nickname_status(bool status);
		void set_registration_status(bool status);

	private:
		Client(Client const& x);
		Client& operator=(Client const& x);

		int _socket;
		std::string _nickname;
		std::string _username;
		std::string _hostname;
		std::string _servername;
		std::string _real_name;
		bool _password_status;
		bool _nickname_status;
		bool _registration_status;
		Modes _modes;
	};
}

#endif
