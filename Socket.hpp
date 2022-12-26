#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdio>
#include <unistd.h>


class Socket
{
    public:
        // Constructor
        Socket();
        // Destructor
        ~Socket();

        // Socket creation and management
        int create(int domain, int type, int protocol);
        int bind(sockaddr const *addr, socklen_t addrlen);
        int listen(int backlog);
        int accept(sockaddr *addr, socklen_t *addrlen);
        int connect(sockaddr const *addr, socklen_t addrlen);
        int close();

        // Socket options
        int setsockopt(int level, int optname, void const *optval, socklen_t optlen);
        int getsockopt(int level, int optname, void *optval, socklen_t *optlen);

        // Address conversion
        int inet_pton(int af, char const *src, void *dst);
        char const *inet_ntop(int af, void const *src, char *dst, socklen_t size);

        // Data transfer
        ssize_t send(void const *buf, size_t len, int flags);
        ssize_t recv(void *buf, size_t len, int flags);

    private:
        // socket descriptor
        int _fd;
};
