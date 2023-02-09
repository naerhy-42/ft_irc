#ifndef __BOT_HPP__
#define __BOT_HPP__

#include <iostream>
#include <sstream>
#include <vector>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

namespace ft
{
	class Bot
	{
		public:
			Bot(void);
			~Bot(void);

			int get_socket(void) const;

			int init(uint16_t port);
			void connect_to_server(std::string const& password) const;
			void get_operator_privileges(void) const;
			void parse_recv(std::string message) const;

		private:
			Bot(Bot const& x);
			Bot& operator=(Bot const& x);

			int _socket;
			std::string _name;
			std::string _endl;
	};
}

#endif
