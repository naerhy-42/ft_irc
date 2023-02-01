#include "../../include/Protocol.hpp"
#include "../../include/Server.hpp"

namespace ft
{
    void Protocol::cmd_quit(Message msg)
    {
        Client &client = _get_client_from_socket(msg.get_socket());
		std::string reply;

		reply = "ERROR :Closing link\r\n";
		add_to_queue(client, reply, 0);
		_server->close_socket_connection(client.get_socket());
    }
}
