#include "../../include/Protocol.hpp"

namespace ft
{
    void Protocol::cmd_topic(ClientMessage const &cmessage)
    {
        std::cout << "enter the function 1" << std::endl;

        // Get the client using the topic command
        Client *client = cmessage.get_client();

        // Extract the parameters from the message
        std::vector<std::string> parameters = cmessage.get_parameters();

        if (!is_client_connected(client))
        {
            send_message_to_client(client, _replies.err_notregistered(client->get_nickname()));
            ignore_socket(client->get_socket());
        }
        // Ensure that the message has at least one parameter (the channel name)
        if (parameters.size() < 1)
        {
            // If there is no channel parameter, send an error message
            send_message_to_client(client, _replies.err_needmoreparams(client->get_nickname(), "TOPIC"));
            return;
        }

        // Extract the channel name from the parameters
        std::string channel_name = parameters[0];

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
            // Check if the client is in the channel
            bool is_in_channel = false;
            Channel &channel = get_channel_from_name(channel_name);
            std::cout << channel.has_client(client) << std::endl;
            if (channel.has_client(client))
                is_in_channel = true;

            if (!is_in_channel)
            {
                // If the client is not in the channel, send an error message
                // TO_DO
                return;
            }
            else
            {
                std::cout << "enter the function 2" << std::endl;

                // Check if the client wants to set or get the topic
                std::string topic = cmessage.get_remainder();
                if (topic.empty())
                {
                    std::cout << " TOPIC in the CHANNEL : " << topic << std::endl;
                    // If the client wants to get the topic, send the topic message
                    if (channel.get_topic().empty())
                    {
                        send_message_to_client(client, _replies.rpl_notopic(client->get_nickname(), channel_name));
                        return;
                    }
                    else
                    {
                        send_message_to_client(client, _replies.rpl_topic(client->get_nickname(), channel_name, channel.get_topic()));
                        return;
                    }
                }
                else
                {
                    std::cout << "enter the function 3  " << topic << std::endl;

                    // Check if the client is a channel operator TEMP
                    bool is_channel_operator = true;
                    // is_channel_operator = target_channel->is_operator(&current_client);
                    if (!is_channel_operator)
                    {
                        // If the client is not a channel operator, send an error message
                        // TO_DO
                        return;
                    }
                    else
                    {
                        std::cout << "enter the funtion 4" << std::endl;
                        // If the client is a channel operator, set the topic and send the topic message
                        channel.set_topic(topic);
                        channel.set_author(client->get_nickname());
                        send_message_to_channel(channel, ":" + client->get_prefix() + " TOPIC " + channel_name + " :" + channel.get_topic() + _IRC_ENDL, client);
                        send_message_to_client(client, ":" + client->get_prefix() + " TOPIC " + channel_name + " :" + channel.get_topic() + _IRC_ENDL);
                    }
                }
            }
        }
    }

}
