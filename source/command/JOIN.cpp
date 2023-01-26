#include "../../include/Protocol.hpp"

namespace ft
{
    void Protocol::cmd_join(Message msg)
    {
        // Get the client joining the channel
        Client &current_client = _get_client_from_socket(msg.get_socket());

        // Extract the parameters from the message
        std::vector<std::string> parameters = msg.get_parameters();

        // Ensure that the message has at least one parameter (the channel name)
        if (parameters.size() < 1)
        {
            // If there is no channel parameter, send an error message
            std::string error = err_needmoreparams(current_client.get_nickname(), "JOIN");
            _buffer.add_to_queue(current_client, error, 0);
            return;
        }

        // Extract the channel name from the parameters
        std::string channel_name = parameters[0];

        // Check if the channel already exists
        bool channel_exists = false;
        for (size_t i = 0; i < _channels.size(); i++)
        {
            if (_channels[i]->get_name() == channel_name)
            {
                channel_exists = true;
                break;
            }
        }
        std::cout << "does channel exists in JOIN : " << channel_exists << std::endl;
        if (channel_exists)
        {
            // Add the client to the channel if it exists
            Channel &channel = _get_channel_from_name(channel_name);
            channel.add_client(current_client);
        }
        else
        {
            bool is_channel_name_corect = is_valid_channel_name(channel_name);
            if (is_channel_name_corect == true)
            {
                // Create a new channel and add the client to it if it doesn't exist
                Channel *new_channel = new Channel(channel_name, current_client);
                new_channel->add_client(current_client);
                _channels.push_back(new_channel);
            }
            else
            {
                return;
                // this error does not exist in protocol
            }
        }

        // Send a JOIN message to the client
        std::string join_msg = ":" + current_client.get_nickname() + " JOIN " + channel_name + "\r\n";
        _buffer.add_to_queue(current_client, join_msg, 0);

        Channel &channel = _get_channel_from_name(channel_name);
        std::string message = ":" + current_client.get_nickname() + " JOIN " + channel.get_name() + "\r\n";

        for (size_t i = 0; i < channel.get_clients().size(); i++)
        {
            int client_socket = channel.get_clients()[i].get_socket();
            _buffer.add_to_queue(_get_client_from_socket(client_socket), message, 1);
        }

        // Send the topic of the channel to the client
        std::string topic = channel.get_topic();
        // if (!topic.empty())
        // {
        //     std::cout << topic << std::endl;
        //     std::string topic_msg = ":irc-forty-two.com 332 " + current_client.get_nickname() + " " + channel_name + " :" + topic + "\r\n";
        //     _buffer.add_to_queue(current_client, topic_msg, 1);
        // }
        if (!topic.empty())
        {
            // message = ":" + channel.get_author() + " TOPIC " + channel_name + " :" + topic + "\r\n";
            message = ":irc-forty-two.com 332 " + current_client.get_nickname() + " " + channel_name + " :" + channel.get_topic() + "\r\n";
            for (size_t i = 0; i < channel.get_clients().size(); i++)
            {
                int client_socket = channel.get_clients()[i].get_socket();
                if (current_client.get_socket() == client_socket)
                    send(client_socket, message.c_str(), message.length(),0);
            }
        }
        return;
    }
}
