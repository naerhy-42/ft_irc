#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include <string>
#include <vector>

namespace ft
{
	class Client
	{
	public:
		Client(int socket);
		// Client(Client const& x);
		// Client& operator=(Client const& x);
		// ~Client(void);

		int get_socket(void) const;
		std::string const &get_nickname(void) const;
		std::string const &get_username(void) const;
		std::string const &get_hostname(void) const;
		std::string const &get_servername(void) const;
		std::string const &get_real_name(void) const;
		bool get_nickname_status(void) const;
		bool get_registration_status(void) const;
		bool get_password_status(void) const;
		bool get_is_global_operator(void) const;
		std::vector<char> const& get_modes(void) const;
		bool has_mode(char mode) const;
		std::string get_modes_str(void) const;

		void set_nickname(std::string const &nickname);
		void set_username(std::string const &username);
		void set_hostname(std::string const &hostname);
		void set_servername(std::string const &servername);
		void set_real_name(std::string const &real_name);
		void set_nickname_status(bool status);
		void set_registration_status(bool status);
		void set_password_status(bool status);
		void set_mode(char sign, char mode);

		Client &operator=(Client const &other);

	private:
		int _socket;
		std::string _nickname;
		std::string _username;
		std::string _hostname;
		std::string _servername;
		std::string _real_name;
		bool _nickname_status;
		bool _registration_status;
		bool _password_status;
		bool _is_global_operator;
		std::vector<char> _modes;
	};

	bool operator==(Client const &lhs, Client const &rhs);

}

#endif
