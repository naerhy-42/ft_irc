#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include <string>

namespace ft
{
	class client
	{
		public:
			client(void);
			// client(client const& x);
			// client& operator=(client const& x);
			// ~client(void);

			// int get_socket(void) const;
			std::string const& get_nickname(void) const;

			void set_nickname(std::string const& nickname);

		private:
			int _socket;
			std::string _nickname;
			// std::string _username;
			// std::stirng _real_name;
			// std::string _hostname;
	};
}

#endif
