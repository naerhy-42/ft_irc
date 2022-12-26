#include "User.hpp"

User::User() : _nick("") , _channels() {}

User::~User() {}

std::string const &User::get_nick() const
{
    return _nick;
}

bool User::is_valid_nick(std::string const &nick)
{
    // Check the length of the nickname
    if (nick.length() < 1 || nick.length() > 9)
        return false;

    // Check if the nickname starts with a digit
    if (isdigit(nick[0]))
        return false;

    // Check if the nickname contains any invalid characters
    for (std::string::size_type i = 0; i < nick.length(); ++i)
    {
        char c = nick[i];
        if (!isalnum(c) && c != '[' && c != ']' && c != '\\' && c != '`')
            return false;
    }

    // Check if the nickname contains any reserved words
    static std::vector<std::string> reserved_words;
    if (reserved_words.empty())
    {
        // we can add reserved words here
        reserved_words.push_back("SERVER");
        reserved_words.push_back("NICK");
        reserved_words.push_back("USER");
        reserved_words.push_back("QUIT");
        reserved_words.push_back("PART");
        reserved_words.push_back("PRIVMSG");
    }
    for (std::vector<std::string>::iterator it = reserved_words.begin(); it != reserved_words.end(); ++it)
    {
        std::string const &word = *it;
        if (nick.find(word) != std::string::npos)
            return false;
    }
    return true;
}


// TROUBLE for the SOCKET MANAGEMENT BUT WE SHOULD DO ONE SOCKET BY CLIENT
// void User::set_nick(std::string const &nick)
// {
//     // Check if the nickname is valid
//     if (!is_valid_nick(nick))
//     {
//         // Send an error message to the user
//         send_error_message(_socket, "", "432", nick, "Erroneous nickname");
//         return;
//     }

//     // Set the nickname
//     _nick = nick;
// }


void User::send_to_client(Socket &socket, std::string const &prefix, std::string const &command, std::vector<std::string> const &params)
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

void User::send_error_message(Socket &socket, std::string const &prefix, std::string const &error_code, std::string const &error_message)
{
    std::vector<std::string> params{ error_code, error_message };
    send_to_client(socket, prefix, "ERROR", params);
}


std::vector<std::string> const &User::channels() const
{
    return _channels;
}

void User::join_channel(std::string const &channel)
{
    _channels.push_back(channel);
}

void User::part_channel(std::string const &channel)
{
    for (std::vector<std::string>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (*it == channel)
        {
            _channels.erase(it);
            break;
        }
    }
}
