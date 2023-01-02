#include "Client.hpp"
#include <iostream>

int main()
{
    Client client;

    // Test connecting to an IPv4 server
    std::string server_ipv4 = "127.0.0.1";
    std::string server_port_ipv4 = "8080";
    std::cout << "Connecting to IPv4 server at " << server_ipv4 << ":" << server_port_ipv4 << std::endl;
    client.connect(server_ipv4, server_port_ipv4);
    std::cout << "Connected to IPv4 server" << std::endl;

    // Disconnect from the server
    client.disconnect();

    // Test connecting to an IPv6 server
    std::string server_ipv6 = "::1";
    std::string server_port_ipv6 = "8080";
    std::cout << "Connecting to IPv6 server at " << server_ipv6 << ":" << server_port_ipv6 << std::endl;
    client.connect(server_ipv6, server_port_ipv6);
    std::cout << "Connected to IPv6 server" << std::endl;

    // Disconnect from the server
    client.disconnect();

    return 0;
}
