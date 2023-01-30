#include "../../include/Protocol.hpp"
#include "../../include/Server.hpp"

namespace ft
{
    void Protocol::cmd_ping(Message msg)
    {
        Client &client = _get_client_from_socket(msg.get_socket());
        std::string pong;

        // IRC clients should normally not send those commands if unregistered...
        if (client.get_password_status())
		{
			pong = "PONG\r\n";
			_buffer.add_to_queue(client, pong, 1);
		}
    }
}
