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

// std::string	Request::getHost() const
// {
// 	return this->http_version;
// }

std::string	Request::getHost() const
{
	return this->host;
}

// std::string Request::getHeaders() const
// {
// 	return this->protocol_headers;
// }

std::string Request::getBody() const
{
	return this->protocol_body;
}

int Request::getCode() const
{
	return this->code;
}

std::map<std::string, std::string> Request::getHeaders() const
{
	return this->headers;
}

// void		Request::setMethod(std::string line)
// {
// 	std::string	method[3] = {"GET", "POST", "DELETE"};
// 	for(int i = 0; i < 3; i++)
// 	{
// 		if(line.find(method[i]) != std::string::npos)
// 			this->method = method[i];
// 	}
// }

// void		Request::setUri(std::string line)
// {
// 	std::string uri;
// 	size_t pos = line.find_first_of("/");
// 	size_t end = line.find("HTTP", pos);
// 	uri.append(line, pos, end - 4);
// 	this->uri = uri;

// 	std::cout << "URI IS : " << this->uri << std::endl;
// }

// void		Request::setHTTP_version(std::string line)
// {
// 	std::string version;
// 	size_t pos = line.find("HTTP");
// 	version.append(line, pos, pos + 8);
// 	this->http_version = version;
// }

void Request::setHTTPversion(std::string line)
{
	this->http_version = line.substr(0, 8);
	if (this->http_version != "HTTP/1.1")
		this->code = 505;
}

std::string Request::setURI(std::string line)
{
	size_t pos = 0;
	std::string temp;

	while(pos != line.find(" ", 1))
		pos++;
	this->uri = line.substr(1, pos);
	temp = line.substr(pos + 1, line.length() - pos);
	return temp;
}

std::string		Request::setMethod(std::string line)
{
	u_long i = 0;
	size_t pos = 0;
	std::string temp;
	std::string	available_methods[3] = {"GET", "POST", "DELETE"};

	while(pos != line.find(" ", 0))
		pos++;
	this->method = line.substr(0, pos);
	for(i = 0; i < available_methods->size(); i++)
	{
		if (this->method == available_methods[i])
			break;
	}
	if (i == 3)
		this->code = 204;
	temp = line.substr(pos, line.length() - pos);
	return temp;
}

void Request::parse_first_line(std::string line)
{
	line = setMethod(line);
	line = setURI(line);
	setHTTPversion(line);
}

void		Request::parseRequest(char *buffer)
{
    std::string		line(buffer);
	size_t prev = 0, pos = 0;
    while(pos != line.find("\n", prev))
		pos++;
	parse_first_line(line.substr(prev, pos - prev));

	// setHeaders(line.substr(prev, pos - prev));
	// setBody(line.substr(prev, pos - prev));

// /r/n/r/n - пустая строка перед body

// Request - status code - при ошибке - 204. 400, 505.   200 - по умолчанию


}



// method 
