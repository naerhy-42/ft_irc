#include "../../include/Protocol.hpp"

namespace ft
{
    void Protocol::cmd_kick(Message msg)
    {
        // Get the client using the kick command
        Client &current_client = _get_client_from_socket(msg.get_socket());

        // Extract the parameters from the message
        std::vector<std::string> parameters = msg.get_parameters();  
    
        // Ensure that the message has at least two parameter (the channel name, the client name)
        if (parameters.size() < 2)
        {
            // If there is no channel parameter, send an error message
            std::string error = err_needmoreparams(current_client.get_nickname(), "KICK");
            add_to_queue(current_client, error, 0);
            return;
        }
        
        // Extract the channel name from the parameters
        std::string channel_name = parameters[0];
        // Extract the nickname from the parameters
        std::string target_nickname = parameters[1];
        std::string reason = msg.get_remainder();

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

        if (channel_exists == false)
		{
			// If the channel does not exist, send an error message
			std::string error = err_nosuchchannel(current_client.get_nickname(), channel_name);
			add_to_queue(_get_client_from_socket(msg.get_socket()), error, 0);
			return ;
		}
        else
        { 
            // Check if the client is operator (global or local)
            bool is_channel_operator = false;
            bool is_global_operator = false;
            Channel* target_channel = _get_channel_from_name(channel_name);
            is_global_operator = current_client.get_is_global_operator();
            is_channel_operator = target_channel->is_operator(&current_client);
            if (is_global_operator == false  && is_channel_operator == false)
            {
                	std::string error = err_chanoprivsneeded(current_client.get_nickname(), channel_name);
			        add_to_queue(_get_client_from_socket(msg.get_socket()), error, 0);
			        return ;
            }
            else
            {
                //Check if target user is in channel
                bool is_in_channel = false;
                Client &target_client = _get_client_from_nickname(target_nickname);
                is_in_channel = target_channel->has_client(&target_client);
                if (is_in_channel == false)
                {
			        std::string error = err_usernotinchannel(current_client.get_nickname(), target_nickname, channel_name);
			        add_to_queue(_get_client_from_socket(msg.get_socket()), error, 0);
			        return ;
                }
                else
                {
			        // Send a message to all clients in the channel that the client has left
                    std::string message = ":" + current_client.get_nickname() + " KICK " + target_channel->get_name() + " " + target_nickname + " :" + reason + "\r\n";
			        for (size_t i = 0; i < target_channel->get_clients().size(); i++)
			        {
			        	int client_socket = target_channel->get_clients()[i]->get_socket();
                        if (current_client.get_socket() != client_socket)
			        	    add_to_queue(client_socket, message, 1);
			        }
			        add_to_queue(current_client, message, 1);
                    target_channel->remove_client(&target_client);
                }
            }
        }
    }
}
