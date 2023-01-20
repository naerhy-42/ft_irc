#include "../../include/Protocol.hpp"

namespace ft
{
	void Protocol::cmd_names(Message msg)
	{
		// Get the client sending the message
		Client &current_client = _get_client_from_socket(msg.get_socket());

		// Extract the parameters from the message
		std::vector<std::string> parameters = msg.get_parameters();

		// Check if the channel exists
		std::string const target_channel_name = parameters[0];
		bool channel_exists = false;
		for (size_t i = 0; i < _channels.size(); i++)
		{
			if (_channels[i]->get_name() == target_channel_name)
			{
				channel_exists = true;
				break;
			}
		}
		std::cout << "channel exists :" << channel_exists << std::endl;
		if (channel_exists)
		{
			// Send the names of all clients in the channel
			Channel &target_channel = _get_channel_from_name(target_channel_name);
			std::vector<std::string> list_of_users;
			std::vector<Client> clients = target_channel.get_clients();
			for (size_t i = 0; i < clients.size(); i++)
			{
				list_of_users.push_back(clients[i].get_nickname());
				std::cout << clients[i].get_nickname() << std::endl;
			}
			std::string names_msg = rpl_namreply("irc.forty-two.com", current_client.get_nickname(), target_channel.get_name(), list_of_users);
			send(msg.get_socket(), names_msg.c_str(), names_msg.size(), 0);
			_buffer.add_to_queue(_get_client_from_socket(msg.get_socket()), names_msg, 1);
			names_msg = rpl_endofnames(current_client.get_nickname(), target_channel.get_name());
			_buffer.add_to_queue(_get_client_from_socket(msg.get_socket()), names_msg, 1);
			return ;
		}
		else
		{
			// If there is no target channel, send an error message
			std::string error = err_nosuchchannel(current_client.get_nickname(), target_channel_name);
			_buffer.add_to_queue(_get_client_from_socket(msg.get_socket()), error, 0);
			return ;
		}
	}
}