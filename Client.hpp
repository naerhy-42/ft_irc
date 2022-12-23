#pragma once

#include <string>
#include "Socket.hpp"
#include "Protocol.hpp"

class Client
{
    public:
    
        Client();
        ~Client();
    
        // Connect to the IRC server
        void connect(std::string const &server, std::string const &port);
    
        // Disconnect from the IRC server
        void disconnect();
    
    private:
        // Socket for the connection to the IRC server
        Socket _socket;
    
        // Protocol handler for the IRC connection : Access to all the message handlers
        Protocol _protocol;
};


// The final goal, code like this:
// client.get_protocol().send_message("NICK mynick");
