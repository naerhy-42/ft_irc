#include "../../include/Protocol.hpp"

namespace ft
{
    void Protocol::cmd_whois(Message msg)
    {
        // Get the client sending the command
        Client &current_client = _get_client_from_socket(msg.get_socket());

        // Extract the parameters from the message
        std::vector<std::string> parameters = msg.get_parameters();

        // Ensure that the message has one parameter (the target nickname)
        if (parameters.size() != 1)
        {
            // If there is no target nickname, send an error message
            std::string error = err_needmoreparams(current_client.get_nickname(), "WHOIS");
            add_to_queue(_get_client_from_socket(msg.get_socket()), error, 0);
            return;
        }

        // Extract the target nickname from the parameters
        std::string target_nickname = parameters[0];

        // Check if the target nickname exists
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
            // Send the WHOIS information if the target exists
            Client &target_client = _get_client_from_nickname(target_nickname);
            std::string whois_info = rpl_whoisuser(current_client.get_nickname(), target_client.get_nickname(), target_client.get_username(), target_client.get_hostname(), target_client.get_real_name());
            add_to_queue(_get_client_from_socket(msg.get_socket()), whois_info, 1);

            // Build the channel string
            std::string channels = "";
            for (size_t i = 0; i < _channels.size(); i++)
            {
                if (_channels[i]->has_client(&target_client))
                {
                    channels += _channels[i]->get_name() + " ";
                }
            }

            if (channels.size() > 0)
            {
                std::string whois_channels = rpl_whoischannels(current_client.get_nickname(), target_client.get_nickname(), channels);
                add_to_queue(_get_client_from_socket(msg.get_socket()), whois_channels, 1);
            }

            // Send the end of WHOIS message
            std::string end_of_whois = rpl_endofwhois(current_client.get_nickname(), target_client.get_nickname());
            add_to_queue(_get_client_from_socket(msg.get_socket()), end_of_whois, 1);
            return;
        }
        else
        {
            // If the target nickname does not exist, send an error message
            std::string error = err_nosuchnick(current_client.get_nickname(), target_nickname);
            send(msg.get_socket(), error.c_str(), error.size(), 0);
            add_to_queue(_get_client_from_socket(msg.get_socket()), error, 1);
            return;
        }
    }
}
