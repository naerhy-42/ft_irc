#include "Client.hpp"

Client::Client() {}
Client::~Client() {}


void Client::connect(std::string const &server, std::string const &port)
{
    // Create a socket
    if (_socket.create(AF_INET, SOCK_STREAM, 0) == -1)
    {
        // An error occurred creating the socket
        return;
    }

    // Set the address and port for the server In this case, the union is used
    //  to store the server address, which may be either an IPv4 address or an
    //  IPv6 address. Using an union avoid the need for reinterpret_cast and the
    //  conditional code that sets the address family and port. 
    union
    {
        sockaddr_in ipv4;
        sockaddr_in6 ipv6;
        sockaddr sa;
    } server_addr;
    if (_socket.inet_pton(AF_INET, server.c_str(), &server_addr.ipv4.sin_addr) == 1)
    {
        // IPv4 address
        server_addr.ipv4.sin_family = AF_INET;
        int port_int = atoi(port.c_str());
        server_addr.ipv4.sin_port = htons(port_int);
    }
    else if (_socket.inet_pton(AF_INET6, server.c_str(), &server_addr.ipv6.sin6_addr) == 1)
    {
        // IPv6 address
        server_addr.ipv6.sin6_family = AF_INET6;
        int port_int = atoi(port.c_str());
        server_addr.ipv6.sin6_port = htons(port_int);
    }
    else
    {
        // Invalid address
        return;
    }

    // Connect to the server
    if (_socket.connect(&server_addr.sa, sizeof(server_addr)) == -1)
    {
        // An error occurred connecting to the server
        return;
    }
}


void Client::disconnect()
{
    _socket.close();
}
