#include "../../include/Protocol.hpp"

namespace ft
{
    void Protocol::cmd_whois(ClientMessage const& cmessage)
    {
        Client *client = cmessage.get_client();
        std::vector<std::string> const& parameters = cmessage.get_parameters();

		if (!is_client_connected(client))
		{
            send_message_to_client(client, _replies.err_notregistered(client->get_nickname()));
            ignore_socket(client->get_socket());
		}
		else if (parameters.size() < 1)
            send_message_to_client(client, _replies.err_nonicknamegiven(client->get_nickname()));
		else if (!is_client_active(parameters[0]))
			send_message_to_client(client, _replies.err_nosuchnick(client->get_nickname(), parameters[0]));
		else
		{
			std::string target_nickname = parameters[0];

			Client* target = get_client_from_name(target_nickname);
			std::string channels_list;

			send_message_to_client(client, _replies.rpl_whoisuser(client->get_nickname(),
					target->get_nickname(), target->get_username(), target->get_hostname(),
					target->get_real_name()));
			channels_list = get_user_channels_list(client);
			if (!channels_list.empty())
				send_message_to_client(client, _replies.rpl_whoischannels(client->get_nickname(),
						target->get_nickname(), channels_list));
			send_message_to_client(client, _replies.rpl_endofwhois(client->get_nickname(),
					target->get_nickname()));
		}
    }
}
