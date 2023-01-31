#include "../../include/Protocol.hpp"

namespace ft
{
    void Protocol::cmd_privmsg(Message msg)
    {
        // Get the client sending the message
        Client &current_client = _get_client_from_socket(msg.get_socket());

        // Extract the parameters from the message
        std::vector<std::string> parameters = msg.get_parameters();

        // Ensure that the message has at least one parameter (the target)
        if (parameters.size() < 1)
        {
            // If there is no target parameter, send an error message
            std::string error = err_needmoreparams(current_client.get_nickname(), "PRIVMSG");
            add_to_queue(current_client, error, 0);
            return;
        }

        // Determine if the target is a user or a channel
        bool is_channel = false;
        if (parameters[0][0] == '#')
        {
            is_channel = true;
        }
        std::cout << "is it a channel PRIVMSG ?  :" << is_channel << std::endl;

        // Extract the message text from the parameters
        std::string message = msg.get_remainder();

        // Send the message to the target/channel
        if (is_channel)
        {
            // Check if the channel exists
            std::string const target_channel_name = parameters[0];
            bool channel_exists = false;
            for (size_t i = 0; i < _channels.size(); i++)
            {
                std::cout << "channel name  :" << _channels[i]->get_name() << std::endl;
                std::cout << _channels[i]->get_name() << std::endl;
                if (_channels[i]->get_name() == target_channel_name)
                {
                    std::cout << "channel name for PRIVMSG  :" << _channels[i]->get_name() << std::endl;
                    channel_exists = true;
                    break;
                }
            }
            std::cout << "does channel exists  ?  :" << channel_exists << std::endl;
            if (channel_exists)
            {
                // Send the message to all clients in the channel
                Channel* target_channel = _get_channel_from_name(target_channel_name);
                std::string send_msg = ":" + current_client.get_nickname() + " PRIVMSG " + target_channel->get_name() + " :" + message + "\r\n";
                for (size_t i = 0; i < target_channel->get_clients().size(); i++)
                {
                    int client_socket = target_channel->get_clients()[i]->get_socket();
                    if (client_socket != current_client.get_socket())
                        add_to_queue(_get_client_from_socket(client_socket), send_msg, 1);
                }
                return;
            }
            else
            {
                // If there is no target channel, send an error message
                std::cout << "leaving here " << std::endl;
                std::string error = err_nosuchchannel(current_client.get_nickname(), target_channel_name);
                add_to_queue(_get_client_from_socket(msg.get_socket()), error, 0);
            }
        }
        else
        {
            // Check if the nickname exists
            std::string const target_nickname = parameters[0];
            bool nickname_exists = false;
            for (size_t i = 0; i < _clients.size(); i++)
            {
                if (_clients[i].get_nickname() == target_nickname)
                {
                    nickname_exists = true;
                    break;
                }
            }
            if (nickname_exists)
            {
                // Send the message if the target exists
                Client &target_client = _get_client_from_nickname(target_nickname);
                std::string send_msg = ":" + current_client.get_nickname() + " PRIVMSG " + target_nickname + " :" + message + "\r\n";
                add_to_queue(target_client, send_msg, 1);
                return;
            }
            else
            {
                // If there is no target nickname, send an error message
                std::string error = err_nosuchnick(current_client.get_nickname(), target_nickname);
                add_to_queue(_get_client_from_socket(msg.get_socket()), error, 0);
                return;
            }
        }
    }
}
