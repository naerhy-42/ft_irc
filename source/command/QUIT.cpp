#include "../../include/Protocol.hpp"
#include "../../include/Server.hpp"

namespace ft
{
    void Protocol::cmd_quit(Message msg)
    {
        Client &client = _get_client_from_socket(msg.get_socket());

        // IRC clients should normally not send those commands if unregistered...
        if (client.get_password_status())
		{
			_server->close_socket_connection(client.get_socket());
			// if client is on a channel, send QUIT command to clients in this channel
		}
    }
}
