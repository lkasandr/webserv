#include "Client.hpp"

Client::Client(int fd):fd(fd)
{}

Client::~Client(){}

int Client::getClientFd() const
{
	return this->fd;
}

