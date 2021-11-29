#include "Request.hpp"

Request::Request()
{
	this->code = 200;
}

Request::~Request()
{}

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

std::string Request::getBody() const
{
	return this->body;
}

int Request::getCode() const
{
	return this->code;
}

std::map<std::string, std::string> Request::getHeaders() const
{
	return this->headers;
}

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
		this->code = 501;
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
	std::string key, value;
	u_long i = 0;

	for (i = 0; i < line.length(); i++) 
	{
  		if (line[i] == ':')
     		break;
  	}
	if (i == line.length())
		this->code = 505;
	else 
	{
		key = line.substr(0, i);
		value = line.substr(i + 2, line.length() - i);
		this->headers.insert(std::make_pair(key, value));
	}
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
	while(!temp.empty() && (temp != "\r"))
	{
		pos++;
		prev = pos;
		while(pos != line.find("\n", prev) && pos != line.length())
			pos++;
		temp = line.substr(prev, pos - prev + 1);
		if (!temp.empty() && (temp != "\r"))
			add_headers(temp);
		if (temp == "\r")
		{
			std::cout << "WRITE BODY" << std::endl;
			break;
		}
	}
	if (temp == "\r")
		std::cout << "WRITE BODY" << std::endl;
}

//CGI 1 or 0 from URI
//проверить каким образом приходит пост (все ли прочитано)
//ошибка substr где-то

// setHeaders(line.substr(prev, pos - prev));
// setBody(line.substr(prev, pos - prev));

// /r/n/r/n - пустая строка перед body

// Request - status code - при ошибке - 204. 400, 505.   
// status code = 200 - по умолчанию

// что вообще приходит из браузера:
// GET / HTTP/1.1
// Host: localhost:8000
// Upgrade-Insecure-Requests: 1
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
// User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_6) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/14.0.3 Safari/605.1.15
// Accept-Language: ru
// Accept-Encoding: gzip, deflate
// Connection: keep-alive

//curl -X POST  http://localhost:8000 -d "name=value" -I
