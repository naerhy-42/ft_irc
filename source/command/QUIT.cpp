#include "Protocol.hpp"
#include "Server.hpp"

namespace ft
{
    void Protocol::cmd_quit(ClientMessage const& cmessage)
    {
        Client* client = cmessage.get_client();
		std::string client_reply = "ERROR :Closing link " + _server.get_hostname() + " (Client Quit)" + _IRC_ENDL;

		send_message_to_client(client, client_reply);
		ignore_socket(client->get_socket());
		if (is_client_connected(client))
		{
			std::string message = ":" + client->get_prefix() + " QUIT :Client Quit" + _IRC_ENDL;

			send_message_to_client_channels(client, message);
			ignore_socket(client->get_socket());
			_server.close_socket_connection(client->get_socket());
		}
    }
}
