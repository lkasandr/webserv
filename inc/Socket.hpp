#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>

class Socket
{
    private:
        int listening_socket_fd;
        int accept_socket_fd;
        int opt;
        struct sockaddr_in addr;
        struct sockaddr_in new_addr;
        socklen_t len;

    public:
        Socket(unsigned short int port);
        ~Socket();
        // void create_socket();
        // void bind_socket(unsigned short int port);
        // void listen_socket();
        int accept_socket();
        int get_listening_socket_fd();
        int get_accept_socket_fd();
        
        int create_socket();
        int bind_socket(unsigned short int port);
        int listen_socket();
        int setting_socket();
};

#endif