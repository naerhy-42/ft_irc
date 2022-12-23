#include "Socket.hpp"

Socket::Socket() {}
Socket::~Socket() {}

// create the socket
int Socket::create(int domain, int type, int protocol)
{
    _fd = socket(domain, type, protocol);
    if (_fd == -1)
    {
        perror("Error creating socket");
        return -1;
    }
    return 0;
}


// bind the socket to an address 
int Socket::bind(sockaddr const *addr, socklen_t addrlen)
{
    int result = ::bind(_fd, addr, addrlen);
    if (result == -1)
    {
        perror("Error binding socket");
        return -1;
    }
    return 0;
}

// listen for incoming connection 
int Socket::listen(int backlog)
{
    int result = ::listen(_fd, backlog);
    if (result == -1)
    {
        perror("Error listening on socket");
        return -1;
    }
    return 0;
}

// accept the connection 
int Socket::accept(sockaddr *addr, socklen_t *addrlen)
{
    int client_fd = ::accept(_fd, addr, addrlen);
    if (client_fd == -1)
    {
        perror("Error accepting connection");
        return -1;
    }
    return client_fd;
}

// establish a connection to a remote server
int Socket::connect(sockaddr const *addr, socklen_t addrlen)
{
    int result = ::connect(_fd, addr, addrlen);
    if (result == -1)
    {
        perror("Error connecting to server");
        return -1;
    }
    return 0;
}

// close the connection
int Socket::close()
{
    int result = ::close(_fd);
    if (result == -1)
    {
        perror("Error closing socket");
        return -1;
    }
    // set the fd at -1 to prevent accidental reuse
    _fd = -1;
    return 0;
}

int Socket::setsockopt(int level, int optname, const void *optval, socklen_t optlen)
{
    int result = ::setsockopt(_fd, level, optname, optval, optlen);
    if (result == -1)
    {
        perror("Error setting socket option");
        return -1;
    }
    return 0;
}

int Socket::getsockopt(int level, int optname, void *optval, socklen_t *optlen)
{
    int result = ::getsockopt(_fd, level, optname, optval, optlen);
    if (result == -1)
    {
        perror("Error getting socket option");
        return -1;
    }
    return 0;
}

int Socket::inet_pton(int af, char const* src, void* dst)
{
    if (af == AF_INET)
    {
        // IPv4 address
        return ::inet_pton(af, src, dst);
    }
    else if (af == AF_INET6)
    {
        // IPv6 address
        return ::inet_pton(af, src, dst);
    }
    else
    {
        // Invalid address family
        return -1;
    }
}

char const* Socket::inet_ntop(int af, void const* src, char* dst, socklen_t size)
{
    if (af == AF_INET)
    {
        // IPv4 address
        return ::inet_ntop(af, src, dst, size);
    }
    else if (af == AF_INET6)
    {
        // IPv6 address
        return ::inet_ntop(af, src, dst, size);
    }
    else
    {
        // Invalid address family
        return nullptr;
    }
}

ssize_t Socket::send(void const* buf, size_t len, int flags)
{
    return ::send(_fd, buf, len, flags);
}


ssize_t Socket::recv(void* buf, size_t len, int flags)
{
    return ::recv(_fd, buf, len, flags);
}
