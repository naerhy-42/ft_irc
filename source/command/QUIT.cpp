#include "../../include/Protocol.hpp"
#include "../../include/Server.hpp"

namespace ft
{
    void Protocol::cmd_quit(Message msg)
    {
        Client &client = _get_client_from_socket(msg.get_socket());

		if (!_is_client_connected(client))
		{
			std::string reply = err_notregistered(client.get_nickname());
			add_to_queue(client, reply, 0);
		}
        if (client.get_password_status())
		{
			_server->close_socket_connection(client.get_socket());
			// if client is on a channel, send QUIT command to clients in this channel
		}
    }
}
