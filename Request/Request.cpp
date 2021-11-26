#include "Request.hpp"


Request::Request()
{
	this->code = 200;
}

Request::~Request()
{

}

std::string	Request::getMethod() const
{
	return this->method;
}

std::string	Request::getUri() const
{
	return this->uri;
}

std::string	Request::getHTTP_version() const
{
	return this->http_version;
}

std::string	Request::getHost() const
{
	return this->host;
}

std::string Request::getHeaders() const
{
	return this->protocol_headers;
}

std::string Request::getBody() const
{
	return this->protocol_body;
}

int Request::getCode() const
{
	return this->code;
}

void		Request::setMethod(std::string line)
{
	std::string	method[3] = {"GET", "POST", "DELETE"};
	for(int i = 0; i < 3; i++)
	{
		if(line.find(method[i]) != std::string::npos)
			this->method = method[i];
	}
}

void		Request::setUri(std::string line)
{
	std::string uri;
	size_t pos = line.find_first_of("/");
	size_t end = line.find("HTTP", pos);
	uri.append(line, pos, end - 4);
	this->uri = uri;
}

void		Request::setHTTP_version(std::string line)
{
	std::string version;
	size_t pos = line.find("HTTP");
	version.append(line, pos, pos + 8);
	this->http_version = version;
}

void		Request::parseRequest(char *buffer)
{
    std::string		line(buffer);
	size_t prev = 0, pos = 0;
    while(pos != line.find("\n", prev))
		pos++;
	this->first_line = line.substr(prev, pos - prev);
	setMethod(this->first_line);
	setUri(this->first_line);
	setHTTP_version(this->first_line);
	// setHeaders();
	// setBody();

}