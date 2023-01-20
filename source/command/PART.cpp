#include "../../include/Protocol.hpp"

namespace ft 
{
    	void Protocol::cmd_part(Message msg)
	{
		// Get the client sending the command
		Client &current_client = _get_client_from_socket(msg.get_socket());
		// Extract the parameters from the message
		std::vector<std::string> parameters = msg.get_parameters();

		// Ensure that the message has one parameter (the target channel)
		if (parameters.size() != 1)
		{
			// If there is no target channel, send an error message
			std::string error = err_needmoreparams(current_client.get_nickname(), "PART");
			_buffer.add_to_queue(_get_client_from_socket(msg.get_socket()), error, 0);
			return ;
		}

		// Extract the target channel from the parameters
		std::string target_channel = parameters[0];

		// Check if the target channel exists
		bool channel_exists = false;
		for (size_t i = 0; i < _channels.size(); i++)
		{
			if (_channels[i]->get_name() == target_channel)
			{
				channel_exists = true;
				break;
			}
		}

		if (channel_exists)
		{
			std::cout << "channel exists ? " << channel_exists << std::endl; 
			// Remove the client from the channel
			Channel &channel = _get_channel_from_name(target_channel);
			channel.remove_client(current_client);
			std::cout << "client :" << current_client.get_nickname() << " removed" << std::endl;

			// Send a message to all clients in the channel that the client has left
			std::string message = ":" + current_client.get_nickname() + " PART " + channel.get_name() + "\r\n";
			for (size_t i = 0; i < channel.get_clients().size(); i++)
			{
				int client_socket = channel.get_clients()[i].get_socket();
				_buffer.add_to_queue(client_socket, message, 1);
			}
			_buffer.add_to_queue(current_client, message, 1);
			for (std::vector<ft::Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it)
			{
				if (*it == &channel)
				{
					if (channel.get_size() == 0)
					{
						delete *it;
						_channels.erase(it);
						break;
					}
				}
			}

			return ;
		}
		else
		{
			// If the channel does not exist, send an error message
			std::string error = err_nosuchchannel(current_client.get_nickname(), target_channel);
			_buffer.add_to_queue(_get_client_from_socket(msg.get_socket()), error, 0);
			return ;
		}
	}
}