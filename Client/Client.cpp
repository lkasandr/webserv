#include "Client.hpp"

Client::Client(int fd):fd(fd)
{
	this->chunk_ready = false;
	this->content_len = 0;
	this->bound = "0\r\n\r\n";
}

Client::Client(int file_fd, bool file ):fd(fd)
{
	this->stream = fdopen(file_fd, "w+");
	this->file_fd = fileno(stream);
	this->chunk_ready = false;
	this->content_len = 0;
	this->bound = "0\r\n\r\n";
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

int Client::getFileFd() const
{
	return this->file_fd;
}

FILE* Client::getStream()
{
	return this->stream;
}

Client::~Client(){}



