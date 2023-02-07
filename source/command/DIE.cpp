#include "Protocol.hpp"

namespace ft
{
    void Protocol::cmd_die(ClientMessage const &cmessage)
    {
        Client *client = cmessage.get_client();

        if (!is_client_connected(client))
        {
            send_message_to_client(client, _replies.err_notregistered(client->get_nickname()));
            ignore_socket(client->get_socket());
        }
        else if (!client->is_global_operator())
			send_message_to_client(client, _replies.err_noprivileges(client->get_nickname()));
        else
            _SERVER_RUNNING = false;
    }
}
