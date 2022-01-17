#include "Client.hpp"

int check_content_length(char *buffer)
{
	std::string line(buffer);
	int len = 0;
	size_t pos;
	pos = line.find("Content-Length: ");
	if (pos != std::string::npos)
	{
		pos = line.find("Content-Length: ") + 16;
		size_t pos_end = line.find("\n", pos);
		line = line.substr(pos, pos_end - pos);
		len = atoi(line.c_str());
	}
	return len;
}

Client::Client(int fd, char *buffer):fd(fd)
{
	std::string tmp(buffer);
	this->chunk_ready = false;
	this->content_len = 0;
	this->chunked = false;
	this->need_body = false;
	if(tmp.find("Content-Length") != std::string::npos)
		this->content_len = check_content_length(buffer);
	if(tmp.find("Transfer-Encoding: chunked") != std::string::npos )
		this->chunked = true;
	if(tmp.find("PUT ") != std::string::npos || tmp.find("POST ") != std::string::npos)
		this->need_body = true;	
	tmp.clear();
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

bool	Client::check_need_body() const
{
	return this->need_body;
}

Client::~Client(){}



