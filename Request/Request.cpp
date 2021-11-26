#include "Request.hpp"


Request::Request()
{
	this->code = 200;
}

Request::~Request()
{

}

// ................................
// ............ГЕТТЕРЫ.............
// ................................

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


// std::string Request::getHeaders() const
// {
// 	return this->protocol_headers;
// }

std::string Request::getHost() const
{
	return this->host;
}

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



// ................................
// ............СЕТТЕРЫ.............
// ................................
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

void Request::add_headers(std::string line)
{
	size_t pos = 0;
	std::string key, value;

	while((pos != line.find(':', 0)) && (pos != std::string::npos))
		pos++;
	// key = line.substr(0, pos);
	// value = line.substr(pos + 1, line.length() - pos);
	// this->headers.insert(std::make_pair(key, value));
}

void		Request::parseRequest(char *buffer)
{
    std::string		line(buffer);
	std::string		temp;
	size_t prev = 0, pos = 0;
    while(pos != line.find("\n", prev))
		pos++;
	temp = line.substr(prev, pos - prev);
	parse_first_line(temp);
	//пока не встретили пустую строку или не дошли до конца лайна:
	while(!temp.empty() && temp != "\r\n\r\n")
	{
		prev = pos + 1;
		while(pos != line.find("\n", prev) && pos != line.length())
			pos++;
		temp = line.substr(prev, pos - prev);
		if (!temp.empty())
			add_headers(temp);
	}


	// setHeaders(line.substr(prev, pos - prev));
	// setBody(line.substr(prev, pos - prev));

// /r/n/r/n - пустая строка перед body

// Request - status code - при ошибке - 204. 400, 505.   200 - по умолчанию


}



// GET / HTTP/1.1
// Host: localhost:8000
// Upgrade-Insecure-Requests: 1
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
// User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_6) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/14.0.3 Safari/605.1.15
// Accept-Language: ru
// Accept-Encoding: gzip, deflate
// Connection: keep-alive
