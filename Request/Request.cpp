#include "Request.hpp"

Request::Request()
{
	this->code = 200;
	this->cgi = 0;
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
	// std::cout << "CODE IN REQUEST: " << code << std::endl;
	return this->code;
}

bool Request::getCGI() const
{
	return this->cgi;
}

std::map<std::string, std::string> Request::getHeaders() const
{
	return this->headers;
}

void Request::setBody(std::string line)
{
	size_t pos = 0;
	pos = line.find("0\r\n\r\n");
	this->body = line.substr(0, pos);
    if(this->body.length() == 0 || this->body.size() == 0 || this->body == "0")
        this->code = 204;
}

void Request::setHTTPversion(std::string line)
{
	this->http_version = line.substr(0, 8);
	// std::cout << "HTTP VERSION: " << this->http_version  << std::endl;
	// std::cout << this->http_version.length()  << std::endl;
	if (this->http_version != "HTTP/1.1")
		this->code = 505;
	// std::cout << "CODE IN REQUEST: " << code << std::endl;
}

std::string Request::setURI(std::string line)
{
	size_t pos = 0;
	std::string temp;
	std::string cgi_indicator;

	pos++;
	if (line[pos] == '/')
	{
		while(pos != line.find(" ", 1))
			pos++;
		this->uri = line.substr(1, pos);
		temp = line.substr(pos + 1, line.length() - pos);
		cgi_indicator = this->uri.substr(0, 9);
		if (cgi_indicator == "/cgi-bin/")
			this->cgi = 1;
	}
	return temp;
}

std::string		Request::setMethod(std::string line)
{
	u_long i = 0;
	size_t pos = 0;
	std::string temp;
	std::string	available_methods[3] = {"GET", "POST", "DELETE"};

	while(pos != line.find(" ", 0) && (pos != line.length()))
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
		// std::cout << "HEADERS: " << key << ": " << value << std::endl;
	}
}

void		Request::parseRequest(char *buffer)
{
    std::string		line(buffer);
	std::string		temp;
	size_t prev = 0, pos = 0;
	// std::cout << "1" << std::endl;
	// std::cout << "BUFFER: " << buffer << std::endl;
    while(pos != line.find("\n", prev))
		pos++;
	// std::cout << "2" << std::endl;
	temp = line.substr(prev, pos - prev);
	// std::cout << "3" << std::endl;
	parse_first_line(temp);
	// std::cout << "4" << std::endl;
	while(!temp.empty() && (temp != "\r\n") && (pos < line.length()))
	{
		pos++;
		prev = pos;
		while(pos != line.find("\n", prev) && pos != line.length())
			pos++;
		temp = line.substr(prev, pos - prev + 1);
		// std::cout << "2" << std::endl;
		// if (!temp.empty() && (temp != "\r\n"))
		if (!temp.empty() && !(temp.find("\r\n\r\n")))
			add_headers(temp);
		// std::cout << "TEMP: " << temp << std::endl;
		// std::cout << "3" << std::endl;
	}
	// std::cout << "4" << std::endl;
	if (getMethod() == "POST")
	{
		prev = pos;
		while(pos != line.length())
			pos++;
		temp = line.substr(prev, pos - prev + 1);
		std::cout << "BODY: " << temp << std::endl;
		setBody(temp);
	}
	// std::cout << "5" << std::endl;
}

std::string Request::check_content_type()
{
	if (headers.find("Content-Type:") != headers.end())
		std::cout << "FIND 180\n";
	return "h";
}


//CGI 1 or 0 from URI
//проверить каким образом приходит пост (все ли прочитано)

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

// curl -X POST  http://localhost:8000 -d "name=value" -I
// curl -d "data=example1&data2=example2" http://localhost:8000
// curl -X POST -F “name=user” -F “password=test” http://localhost:8000
