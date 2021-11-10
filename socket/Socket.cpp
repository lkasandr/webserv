#include "Socket.hpp"

void Socket::create_socket()
{
    this->listening_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->listening_socket_fd == -1)
    {
        std::cerr << "Error in socket creation." << std::endl;
		std::exit(EXIT_FAILURE);
    }
}

int Socket::get_listening_socket_fd()
{
    return this->listening_socket_fd;
}

int Socket::get_accept_socket_fd()
{
    return this->accept_socket_fd;
}

void Socket::bind_socket(unsigned short int port)
{
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(port);
    this->addr.sin_addr.s_addr = htonl(INADDR_ANY);
    setsockopt(get_listening_socket_fd(), SOL_SOCKET, SO_REUSEADDR, &this->opt, sizeof(this->opt));
    if (bind(get_listening_socket_fd(), (struct sockaddr*)&this->addr, sizeof(this->addr)) != 0)
    {
        std::cerr << "Error in getting socket address." << std::endl;
		std::exit(EXIT_FAILURE);
    }
}

void Socket::listen_socket()
{
    if (listen(get_listening_socket_fd(), 128) == -1)
    {
        std::cerr << "Error in putting into listening mode." << std::endl;
		std::exit(EXIT_FAILURE);
    }
}

int Socket::accept_socket()
{
    this->len = sizeof(this->new_addr);
    this->accept_socket_fd = accept(get_listening_socket_fd(), (struct sockaddr*)&this->new_addr, &this->len);
    if (this->accept_socket_fd == -1)
    {
        std::cerr << "Connection error." << std::endl;
		std::exit(EXIT_FAILURE);
    }
    return (this->accept_socket_fd);
}

Socket::Socket(unsigned short int port)
{
        this->opt = 1;
        Socket::create_socket();
        Socket::bind_socket(port);
        Socket::listen_socket();
}

Socket::~Socket()
{
    close(this->listening_socket_fd);
    close(this->accept_socket_fd);
}