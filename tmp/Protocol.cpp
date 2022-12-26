#include "Protocol.hpp"

Protocol::Protocol() {}
Protocol::~Protocol() {}

void Protocol::handle_command(Socket &socket, std::string const &prefix, std::string const &command, std::array<std::string, 2> const &params)
{
    Command cmd = string_to_command(command);
    switch (cmd)
    {
        case Command::NICK:
            handle_nick_command(socket, prefix, command, params);
            break;
        case Command::USER:
            handle_user_command(socket, prefix, command, params);
            break;
        case Command::JOIN:
            handle_join_command(socket, prefix, command, params);
            break;
        case Command::PART:
            handle_part_command(socket, prefix, command, params);
            break;
        case Command::PRIVMSG:
            handle_privmsg_command(socket, prefix, command, params);
            break;
        default:
            break;
    }
}

Protocol::Command Protocol::string_to_command(std::string const &command) const
{
    if (command == "NICK")
        return Command::NICK;
    if (command == "USER")
        return Command::USER;
    if (command == "JOIN")
        return Command::JOIN;
    if (command == "PART")
        return Command::PART;
    if (command == "PRIVMSG")
        return Command::PRIVMSG;
}

void Protocol::send_to_client(Socket &socket, std::string const &prefix, std::string const &command, std::vector<std::string> const &params)
{
    // Build the message to be sent to the client
    std::string message = ":";
    message += prefix;
    message += " ";
    message += command;
    for (std::string const &param : params)
    {
        message += " ";
        message += param;
    }
    message += "\r\n";

    // Send the message to the client
    socket.send(message.c_str(), message.size(), 0);
}

void Protocol::send_error_message(Socket &socket, std::string const &prefix, std::string const &error_code, std::string const &error_message)
{
    std::vector<std::string> params{ error_code, error_message };
    send_to_client(socket, prefix, "ERROR", params);
}

bool Protocol::is_valid_nick(std::string const &nick) const
{
    // Check each character in the nickname
    for (std::size_t i = 0; i < nick.size(); ++i)
    {
        char c = nick[i];
        if (!std::isalnum(c) && c != '-' && c != '_')
        {
            // The character is not a letter, digit, or '-' or '_', so the nickname is not valid
            return false;
        }
    }

    // All characters in the nickname are valid, so the nickname is valid
    return true;
}


// void Protocol::handle_nick_command(Socket &socket, std::string const &prefix, std::array<std::string, 2> const &params)
// {
//     // Validate the number of parameters
//     if (params.size() != 1)
//     {
//         // Send an error message back to the client
//         send_error_message(socket, prefix, "461", "NICK", "Not enough parameters");
//         return;
//     }

//     // Extract the new nickname from the parameters
//     std::string new_nick = params[0];

//     // Validate the new nickname 
//     if (!is_valid_nick(new_nick))
//     {
//         // Send an error message back to the client
//         send_error_message(socket, prefix, "432", new_nick, "Erroneous nickname");
//         return;
//     }

//     // Check if the new nickname is already in use
//     if (is_nick_in_use(new_nick)) // data structure of user nick ?
//     {
//         // Send an error message back to the client
//         send_error_message(socket, prefix, "433", new_nick, "Nickname is already in use");
//         return;
//     }

//     // Change the nickname for the user
//     change_nick(prefix, new_nick);
// }
