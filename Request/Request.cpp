#include "Request.hpp"


Request::Request()
{

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

void		Request::parseRequest(char *buffer)
{
    std::string		line(buffer);
	size_t prev = 0, pos = 0;
    while(pos != line.find("\n", prev)) /*!= std::string::npos)*/
		pos++;
	this->first_line = line.substr(prev, pos - prev);
	setMethod(this->first_line);
	setUri(this->first_line);
	
	
	// do
    // {
    //     pos = line.find("\n", prev);
    //     if (pos == std::string::npos)
	// 		pos = line.length();
    //     this->first_line = line.substr(prev, pos - prev);
    //     // if (!serv.empty())
	// 	// 	servers.push_back(serv);
    //     // prev = pos + 7;
    // }
    // while (pos < line.length() && prev < line.length());
	std::cout << this->first_line << std::endl;
	std::cout << this->method << std::endl;
	std::cout << this->uri << std::endl;
}