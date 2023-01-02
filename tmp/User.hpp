#pragma once

#include <string>
#include <vector>
#include "Socket.hpp"

class User
{
    public:
        User();
        ~User();
    
        // Nickname
        std::string const &get_nick() const;
        void    set_nick(std::string const &nick);
        bool    is_valid_nick(std::string const &nick);
        // Channels
        std::vector<std::string> const &channels() const;
        void    join_channel(std::string const &channel);
        void    part_channel(std::string const &channel);
    
        // Error message
        void    send_to_client(Socket &socket, std::string const &prefix, std::string const &command, std::vector<std::string> const &params);
        void    send_error_message(Socket &socket, std::string const &prefix, std::string const &error_code, std::string const &error_message);
    
    private:
        std::string                 _nick;
        std::vector<std::string>    _channels;
};
