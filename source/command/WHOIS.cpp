#include "../../include/Protocol.hpp"

namespace ft
{
    void Protocol::cmd_whois(ClientMessage const& cmessage)
    {
        // Get the client sending the command
        Client *client = cmessage.get_client();


        // Extract the parameters from the message
        std::vector<std::string> parameters = cmessage.get_parameters();

		if (!is_client_connected(client))
		{
            send_message_to_client(client, _replies.err_notregistered(client->get_nickname()));
            ignore_socket(client->get_socket());
		}

        // Ensure that the message has one parameter (the target nickname)
        if (parameters.size() != 1)
        {
            send_message_to_client(client, _replies.err_needmoreparams(client->get_nickname(), "WHOIS"));
        }

        // Extract the target nickname from the parameters
        std::string target_nickname = parameters[0];

        // Check if the target nickname exists
        bool nickname_exists = false;
        for (size_t i = 0; i < _clients.size(); i++)
        {
            if (_clients[i]->get_nickname() == target_nickname)
            {
                nickname_exists = true;
                break;
            }
        }

        if (nickname_exists)
        {
            // Send the WHOIS information if the target exists
            Client *target_client = get_client_from_name(target_nickname);
            std::string whois_info = _replies.rpl_whoisuser("irc-forty-two.com",
                                                             target_client->get_nickname(),
                                                             target_client->get_username(), 
                                                             target_client->get_hostname(), 
                                                             target_client->get_real_name());
           
            send_message_to_client(client, whois_info);

            // Build the channel string
            std::string channels = "";
            for (size_t i = 0; i < _channels.size(); i++)
            {
                if (_channels[i].has_client(target_client))
                {
                    channels += _channels[i].get_name() + " ";
                }
            }

            if (channels.size() > 0)
            {
                std::string whois_channels = _replies.rpl_whoischannels("irc-forty-two.com",
                                                                         target_client->get_nickname(),
                                                                          channels);
                send_message_to_client(client, whois_channels);
            }

            // Send the end of WHOIS message
            std::string end_of_whois = _replies.rpl_endofwhois("irc-forty-two.com",
                                                                 target_client->get_nickname());
            send_message_to_client(client, end_of_whois);
        }
        else
        {
            // If the target nickname does not exist, send an error message
			send_message_to_client(client, _replies.err_nosuchnick(client->get_nickname(), target_nickname));
        }
    }
}
