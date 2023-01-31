#include "../../include/Protocol.hpp"

namespace ft
{
    void Protocol::cmd_topic(Message msg)
    {
        // Get the client using the topic command
        Client &current_client = _get_client_from_socket(msg.get_socket());

        // Extract the parameters from the message
        std::vector<std::string> parameters = msg.get_parameters();

        // Ensure that the message has at least one parameter (the channel name)
        if (parameters.size() < 1)
        {
            // If there is no channel parameter, send an error message
            std::string error = err_needmoreparams(current_client.get_nickname(), "TOPIC");
            add_to_queue(current_client, error, 0);
            return;
        }

        // Extract the channel name from the parameters
        std::string channel_name = parameters[0];

        // Check if the channel exists
        bool channel_exists = false;
        for (size_t i = 0; i < _channels.size(); i++)
        {
            if (_channels[i]->get_name() == channel_name)
            {
                channel_exists = true;
                break;
            }
        }

        if (!channel_exists)
        {
            // If the channel does not exist, send an error message
            std::string error = err_nosuchchannel(current_client.get_nickname(), channel_name);
            add_to_queue(_get_client_from_socket(msg.get_socket()), error, 0);
            return;
        }
        else
        {
            // Check if the client is in the channel
            bool is_in_channel = false;
            Channel* target_channel = _get_channel_from_name(channel_name);
            is_in_channel = target_channel->has_client(&current_client);
            if (!is_in_channel)
            {
                // If the client is not in the channel, send an error message
                std::string error = err_notonchannel(current_client.get_nickname(), channel_name);
                add_to_queue(_get_client_from_socket(msg.get_socket()), error, 0);
                return;
            }
            else
            {
                // Check if the client wants to set or get the topic
                std::string topic = msg.get_remainder();
                if (topic.empty())
                {
                    std::cout << " TOPIC in the CHANNEL : " << topic << std::endl;
                    // If the client wants to get the topic, send the topic message
                    if (target_channel->get_topic().empty())
                    {
                        std::string message = ":irc.example.com 331 " + current_client.get_nickname() + " " + target_channel->get_name() + " :No topic is set.\r\n";
                        add_to_queue(current_client, message, 1);
                    }
                    else
                    {
                        std::string message = ":irc-forty-two.com 332 " + current_client.get_nickname() + " " + channel_name + " :" + target_channel->get_topic() + "\r\n";
                        add_to_queue(current_client, message, 1);
                    }
                }
                else
                {
                    // Check if the client is a channel operator
                    bool is_channel_operator = false;
                    is_channel_operator = target_channel->is_operator(&current_client);
                    if (!is_channel_operator)
                    {
                        // If the client is not a channel operator, send an error message
                        std::string error = err_chanoprivsneeded(current_client.get_nickname(), target_channel->get_name()) + "\r\n";
                        add_to_queue(_get_client_from_socket(msg.get_socket()), error, 0);
                        return;
                    }
                    else
                    {
                        // If the client is a channel operator, set the topic and send the topic message
                        target_channel->set_topic(topic);
                        target_channel->set_author(current_client.get_nickname());
                        std::string message = ":" + current_client.get_nickname() + "!" + current_client.get_username() + "@" + current_client.get_hostname() + " TOPIC " + channel_name + " :" + topic + "\r\n";
                        for (size_t i = 0; i < target_channel->get_clients().size(); i++)
                        {
                            int client_socket = target_channel->get_clients()[i]->get_socket();
                            if (current_client.get_socket() != client_socket)
                                add_to_queue(_get_client_from_socket(client_socket), message, 1);
                        }
                        add_to_queue(current_client, message, 1);
                    }
                }
            }
        }
    }
}
