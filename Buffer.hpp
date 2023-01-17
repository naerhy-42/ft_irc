#ifndef __BUFFER_HPP__
#define __BUFFER_HPP__

#include <string>
#include <vector>

#include "Client.hpp"

namespace ft
{
	class Server;

	class Buffer
	{
		public:
			Buffer(Server* server);
			// Buffer(Buffer const& x);
			// Buffer& operator=(Buffer const x);
			// ~Buffer(void);

			void add_to_queue(Client client, std::string message, int index);
			void clear(void);
			void send_messages(void);

			struct Temp
			{
				Temp(Client client, std::string message, int index);

				Client client;
				std::string message;
				int index;
				bool to_send;
			};

		private:
			std::vector<Temp> _queue;
			Server* _server;

			void _set_messages_as_invalid(int socket);
	};
}

#endif
