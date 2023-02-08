#include "Protocol.hpp"

namespace ft
{
	void Protocol::cmd_kill(ClientMessage const& cmessage)
	{
		Client* client = cmessage.get_client();
        std::vector<std::string> const& parameters = cmessage.get_parameters();

        if (!is_client_connected(client))
        {
            send_message_to_client(client, _replies.err_notregistered(client->get_nickname()));
            ignore_socket(client->get_socket());
        }
		else if (parameters.size() < 1)
            send_message_to_client(client, _replies.err_needmoreparams(client->get_nickname(), "KILL"));
        else if (!client->is_global_operator())
			send_message_to_client(client, _replies.err_noprivileges(client->get_nickname()));
		else if (!is_client_active(parameters[0]))
			send_message_to_client(client, _replies.err_nosuchnick(client->get_nickname(), parameters[0]));
		else
		{
			Client* target = get_client_from_name(parameters[0]);
			std::string message = ":" + client->get_prefix() + " KILL " + target->get_nickname() + _IRC_ENDL;

			send_message_to_client(target, message);
			cmd_quit(ClientMessage(target, "QUIT"));
		}
	}
}
