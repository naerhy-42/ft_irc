#include "../../include/Protocol.hpp"

namespace ft
{

    void Protocol::cmd_invite(Message msg)
    {
        // Get the client using the invite command
        Client &current_client = _get_client_from_socket(msg.get_socket());

        // Extract the parameters from the message
        std::vector<std::string> parameters = msg.get_parameters();

		if (!_is_client_connected(current_client))
		{
			std::string reply = err_notregistered(current_client.get_nickname());
			add_to_queue(current_client, reply, 0);
		}
        // Ensure that the message has at least two parameters (the nickname, the channel name)
        if (parameters.size() < 2)
        {
            // If there is no nickname or channel parameter, send an error message
            std::string error = err_needmoreparams(current_client.get_nickname(), "INVITE");
            add_to_queue(current_client, error, 0);
            return;
        }

        // Extract the nickname from the parameters
        std::string target_nickname = parameters[0];
        // Extract the channel name from the parameters
        std::string channel_name = parameters[1];

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
            // Check if the client is channel operator
            bool is_channel_operator = false;
            Channel* target_channel = _get_channel_from_name(channel_name);
            is_channel_operator = target_channel->is_operator(&current_client);
            if (!is_channel_operator)
            {
                // If the client is not a channel operator, send an error message
                std::string error = err_chanoprivsneeded(current_client.get_nickname(), target_channel->get_name());
                add_to_queue(_get_client_from_socket(msg.get_socket()), error, 0);
                return;
            }
            else
            {
                // Check if the target client exists
                bool target_exists = false;
                for (size_t i = 0; i < _clients.size(); i++)
                {
                    if (_clients[i].get_nickname() == target_nickname)
                    {
                        target_exists = true;
                        break;
                    }
                }

                if (!target_exists)
                {
                    // If the target client does not exist, send an error message
                    std::string error = err_nosuchnick(current_client.get_nickname(), target_nickname);
                    add_to_queue(_get_client_from_socket(msg.get_socket()), error, 0);
                    return;
                }
                else
                {
                    // Get the target client
                    Client &target_client = _get_client_from_nickname(target_nickname);
                    // Send an invitation message to the target client
                    std::string message = ":" + current_client.get_nickname() + " INVITE " + target_nickname + " " + channel_name + "\r\n";
                    add_to_queue(target_client, message, 1);

                    // Send a confirmation message to the inviting client
                    message = rpl_inviting(current_client.get_nickname(), target_channel->get_name());
                    add_to_queue(current_client, message, 1);
                }
            }
        }
    }
}
