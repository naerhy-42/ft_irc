#include "Protocol.hpp"
#include "Server.hpp"

namespace ft
{
    void Protocol::cmd_quit(ClientMessage const& cmessage)
    {
        Client* client = cmessage.get_client();
		std::string client_reply;

		if (cmessage.get_remainder().empty())
			client_reply = "ERROR :Closing link " + client->get_hostname() + _IRC_ENDL;
		else
			client_reply = "ERROR :Closing link " + client->get_hostname() + " :" + cmessage.get_remainder() + _IRC_ENDL;
		send_message_to_client(client, client_reply);
		ignore_socket(client->get_socket());
		if (is_client_connected(client))
		{
			std::string message;

			message = ":" + client->get_prefix() + " QUIT :Client Quit" + _IRC_ENDL;
			send_message_to_client_channels(client, message);
		}
		// _server.close_socket_connection(client->get_socket());
    }
}
