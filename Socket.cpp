#include "Socket.hpp"

Socket::Socket(int domain, int type, int protocol, int port)
{
	this->address.sin_family = domain;
	this->address.sin_addr.s_addr = htonl(INADDR_ANY);
	this->address.sin_port = htons(port);
	this->server_fd = socket(domain, type, protocol);
}

// address.sin_addr.s_addr = htonl(INADDR_ANY); 
// address.sin_port = htons(PORT);