#pragma once

#include <cstdint>
#include <array>
#include <string>
#include <vector>
#include "Socket.hpp"
#include <cctype>

class Protocol
{
public:
    // Constructor
    Protocol();
    ~Protocol();

    // IRC command enumeration
    enum class Command : std::uint8_t
    {
        NICK,
        USER,
        JOIN,
        PART,
        PRIVMSG
    };

    // IRC command handling functions
    void handle_command(Socket &socket, std::string const &prefix, std::string const &command, std::array<std::string, 2> const &params);
    Command string_to_command(std::string const &command) const;
    
    // message sender
    void send_error_message(Socket &socket, std::string const &prefix, std::string const &error_code, std::string const &error_message);
    void send_to_client(Socket &socket, std::string const &prefix, std::string const &command, std::vector<std::string> const &params);

private:
    // valid nick
    // bool is_valid_nick(std::string const &nick) const;
    // bool is_nick_in_use(std::string const &nick) const;


    // IRC command handling functions
    // void handle_nick_command(Socket &socket, std::string const &prefix, std::string const &command, std::array<std::string, 2> const &params);
    // void handle_user_command(Socket &socket, std::string const &prefix, std::string const &command, std::array<std::string, 2> const &params);
    // void handle_join_command(Socket &socket, std::string const &prefix, std::string const &command, std::array<std::string, 2> const &params);
    // void handle_part_command(Socket &socket, std::string const &prefix, std::string const &command, std::array<std::string, 2> const &params);
    // void handle_privmsg_command(Socket &socket, std::string const &prefix, std::string const &command, std::array<std::string, 2> const &params);
};
