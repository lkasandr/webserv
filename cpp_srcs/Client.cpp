#include "Client.hpp"

Client::Client(int fd):fd(fd)
{
	this->chunk_ready = false;
	this->content_len = 0;
}

void Client::setContentLen(int len)
{
	this->content_len = len;
}

int Client::getContentLen() const
{
	return this->content_len;
}

int Client::getClientFd() const
{
	return this->fd;
}

Client::~Client(){}



