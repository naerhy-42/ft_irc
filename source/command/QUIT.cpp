#include "../../include/Protocol.hpp"
#include "../../include/Server.hpp"

namespace ft
{
    void Protocol::cmd_quit(Message msg)
    {
        Client &client = _get_client_from_socket(msg.get_socket());
		std::string reply;
		std::string message;

		reply = "ERROR :Closing link\r\n";
		add_to_queue(client, reply, 0);
		if (_is_client_connected(client))
		{
			std::vector<Channel*>::const_iterator cit;
			message = ":" + client.get_nickname() + "!" + client.get_username() + "@"
					+ client.get_hostname() + " QUIT :" + msg.get_remainder() + "\r\n";

			for (cit = _channels.begin(); cit != _channels.end(); cit++)
			{
				std::vector<Client*>::const_iterator citt;

				for (citt = (*cit)->get_clients().begin(); citt != (*cit)->get_clients().end(); citt++)
				{
					if (*(*citt) == client)
						send_msg_to_channel_clients(*(*cit), &client, message);
				}
			}
		}

		std::vector<Channel*> channels_to_delete;
        for (std::vector<ft::Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it)
		{
            Channel* channel = *it;
			if (channel->has_client(&client))
			{
                channel->remove_client(&client);
				if (channel->get_size() == 0)
				{
                    channels_to_delete.push_back(channel);
				}
			}
		}
        for (std::vector<Channel*>::iterator channel_it = channels_to_delete.begin(); channel_it != channels_to_delete.end(); ++channel_it)
        {
            Channel *channel = *channel_it;
            delete channel;
        }

		_server.close_socket_connection(client.get_socket());
    }
}
