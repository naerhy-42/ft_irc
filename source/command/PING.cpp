#include "../../include/Protocol.hpp"
#include "../../include/Server.hpp"

namespace ft
{
    void Protocol::cmd_ping(Message msg)
    {
        Client &client = _get_client_from_socket(msg.get_socket());
        std::string pong;

		if (!_is_client_connected(client))
		{
			std::string reply = err_notregistered(client.get_nickname());
			add_to_queue(client, reply, 0);
		}
        if (client.get_password_status())
		{
			pong = "PONG\r\n";
			add_to_queue(client, pong, 1);
		}
    }
}
