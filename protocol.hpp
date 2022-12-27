#ifndef __PROTOCOL_HPP__
#define __PROTOCOL_HPP__

#include <map>
#include <string>

#include "message.hpp"

namespace ft
{
	class protocol
	{
		private:
			typedef std::string (protocol::*fncts)(void);

		public:
			protocol(void);
			~protocol(void);

			void parse_client_input(std::string& client_msg);
			void handle_message(message msg);

			std::string nick_function(void);

		private:
			protocol(protocol const& x);
			protocol& operator=(protocol const& x);

			std::map<std::string, fncts> _functions;
	};
}

#endif
