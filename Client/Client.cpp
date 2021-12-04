#include "Client.hpp"

Client::Client(int fd):fd(fd)
{
	this->chunk_ready = false;
}

Client::~Client(){}

int Client::getClientFd() const
{
	return this->fd;
}

