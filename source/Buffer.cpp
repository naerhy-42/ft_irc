#include "../include/Buffer.hpp"
#include "../include/Server.hpp"

namespace ft
{
	Buffer::Temp::Temp(Client client, std::string message, int index)
		: client(client), message(message), index(index), to_send(true) {}

	Buffer::Buffer(Server* server) : _server(server) {}

	void Buffer::add_to_queue(Client client, std::string message, int index)
	{
		Temp x(client, message, index);
		_queue.push_back(x);
	}

	void Buffer::clear(void)
	{
		_queue.clear();
	}

	void Buffer::send_messages(void)
	{
		ssize_t x;

		for (size_t i = 0; i < _queue.size(); i++)
		{
			if (_queue[i].to_send)
			{
				x = send(_queue[i].client.get_socket(), _queue[i].message.c_str(), _queue[i].message.size(), 0);
				if (x == -1 || _queue[i].index <= 0)
				{
					_set_messages_as_invalid(_queue[i].client.get_socket());
					if (_queue[i].index == -1)
						_server->close_socket_connection(_queue[i].client.get_socket());
				}
			}
		}
	}

	void Buffer::_set_messages_as_invalid(int socket)
	{
		for (size_t i = 0; i < _queue.size(); i++)
		{
			if (_queue[i].client.get_socket() == socket)
				_queue[i].to_send = false;
		}
	}
}
