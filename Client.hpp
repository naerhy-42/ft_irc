#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include <string>

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
			std::string const& get_nickname(void) const;
			std::string const& get_username(void) const;
			std::string const& get_hostname(void) const;
			std::string const& get_real_name(void) const;
			bool get_registration_status(void) const;

			void set_nickname(std::string const& nickname);
			void set_username(std::string const& username);
			void set_hostname(std::string const& hostname);
			void set_real_name(std::string const& real_name);
			void set_registration_status(bool status);

		private:
			int _socket;
			std::string _nickname;
			std::string _username;
			std::string _hostname;
			std::string _real_name;
			bool _registration_status;
	};
}

#endif
