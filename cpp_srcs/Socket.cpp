#include "Socket.hpp"

int Socket::create_socket()
{
    this->listening_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->listening_socket_fd == -1)
    {
        std::cerr << "Error in socket creation." << std::endl;
		return (-1);
    }
    return (0);
}

int Socket::get_listening_socket_fd()
{
    return this->listening_socket_fd;
}

int Socket::get_accept_socket_fd()
{
    return this->accept_socket_fd;
}

int Socket::bind_socket(unsigned short int port)
{
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(port);
    this->addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (setsockopt(this->listening_socket_fd, SOL_SOCKET, SO_REUSEADDR, &this->opt, sizeof(this->opt)) < 0)
    {
        std::cerr << "Error in setsockopt." << std::endl;
        return (-1);
    }
    if (bind(this->listening_socket_fd, (struct sockaddr*)&this->addr, sizeof(this->addr)) != 0)
    {
        std::cerr << "\033[31mError in getting socket address. \033[0m" << std::endl;
        return (-1);
    }
    return 0;
}

int Socket::listen_socket()
{
    if (listen(get_listening_socket_fd(), SOMAXCONN) == -1)
    {
        std::cerr << "Error in putting into listening mode." << std::endl;
        return (-1);
    }
    return 0;
}

int Socket::accept_socket()
{
    this->len = sizeof(this->new_addr);
    this->accept_socket_fd = accept(get_listening_socket_fd(), (struct sockaddr*)&this->new_addr, &this->len);
    if (this->accept_socket_fd == -1)
    {
        std::cerr << "Connection error." << std::endl;
		exit(1);
    }
    return (this->accept_socket_fd);
}



Socket::Socket(unsigned short int port) : port(port)
{
        this->opt = 1;
}

Socket::~Socket()
{}

int Socket::setting_socket()
{
    if (Socket::create_socket() == -1)
        return (-1);
    if (Socket::bind_socket(this->port) == -1)
        return (-1);
    if (Socket::listen_socket() == -1)
        return (-1);
    return 0;
}