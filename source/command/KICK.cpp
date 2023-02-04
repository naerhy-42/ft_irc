#include "../../include/Protocol.hpp"

namespace ft
{
       void Protocol::cmd_kick(ClientMessage const &cmessage)
    {
        // Get the client using the kick command
        Client *client = cmessage.get_client();

        // Extract the parameters from the message
        std::vector<std::string> parameters = cmessage.get_parameters();

        if (!is_client_connected(client))
        {
            send_message_to_client(client, _replies.err_notregistered(client->get_nickname()));
            ignore_socket(client->get_socket());
        }
        // Ensure that the message has at least two parameter (the channel name, the client name)
        if (parameters.size() < 2)
        {
            // If there is no channel parameter, send an error message
            send_message_to_client(client, _replies.err_needmoreparams(client->get_nickname(), "KICK"));
            return;
        }

        // Extract the channel name from the parameters
        std::string channel_name = parameters[0];

        // Extract the nickname from the parameters
        std::string target_nickname = parameters[1];

        // Extract the reason from the parameters (optional)
        std::string reason = cmessage.get_remainder();

        // Check if the channel exists
        bool channel_exists = false;
        if (is_channel_active(channel_name))
            channel_exists = true;

        if (!channel_exists)
        {
            // If the channel does not exist, send an error message
            send_message_to_client(client, _replies.err_nosuchchannel(client->get_nickname(), channel_name));
            return;
        }
        else
        {
            // Check if the client is operator (global or local)
            bool is_channel_operator = false;
            // bool is_global_operator = false;
            Channel &channel = get_channel_from_name(channel_name);
            // is_global_operator = client->get_is_global_operator();
            is_channel_operator = channel.is_operator(client);
            if (is_channel_operator == false)
            {
                send_message_to_client(client, _replies.err_chanoprivsneeded(client->get_nickname(), channel_name));
                return;
            }
            else
            {
                // Check if target user is in channel
                bool is_in_channel = false;
                Client *target_client = get_client_from_name(target_nickname);
                is_in_channel = channel.has_client(target_client);
                if (is_in_channel == false)
                {
                    send_message_to_client(client, ":irc.forty-two.com 441 " + client->get_nickname() + " " + target_nickname + " " + channel_name + " :They aren't on that channel\r\n");
                    return;
                }
                else
                {
                    // Send the kick message to the channel and to the kicker
                    send_message_to_channel(channel, ":" + client->get_prefix() + " KICK " + channel_name + " " + target_nickname + " :" + reason + _IRC_ENDL, client);
                    send_message_to_client(client, ":" + client->get_prefix() + " KICK " + channel_name + " " + target_nickname + " :" + reason + _IRC_ENDL);

                    // Remove the client (kicked)
                    channel.remove_client(target_client);
                }
            }
        }
    }
}
