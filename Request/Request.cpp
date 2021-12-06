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
	if (this->http_version != "HTTP/1.1")
		this->code = 505;
	std::cout << "HTTP VERSION: " << this->http_version << std::endl;
	std::cout << "CODE IS " << this->code << std::endl;
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
		{
			this->cgi = 1;
		}
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
	{
		this->code = 505;
		std::cout << "????????????" << std::endl;
		std::cout << "LINE: " << line << std::endl;
	}
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
	while(!temp.empty() && (temp != "\r\n") && (pos < line.length()))
	{
		pos++;
		prev = pos;
		while(pos != line.find("\n", prev) && pos != line.length())
			pos++;
		temp = line.substr(prev, pos - prev + 1);
		if (!(temp.find("\r\n\r\n")))    
		{
			add_headers(temp);
		}
	}
	if (getMethod() == "POST")
	{
		prev = pos;
		while(pos != line.length())
			pos++;
		temp = line.substr(prev, pos - prev + 1);
		// std::cout << "BODY: " << temp << std::endl;
		setBody(temp);
	}
	// для вывода this->headers:
	// std::cout << "\033[35mMethod: " << getMethod() << "\033[0m" << std::endl;
	// std::cout << "\033[35mURI: " << getUri() << "\033[0m" << std::endl;
	// std::cout << "\033[35mHTTP Version: " << getHTTP_version() << "\033[0m" << std::endl;
	std::map<std::string, std::string>::iterator it;
	std::cout << "HEADERS: " << std::endl;
	for (it=this->headers.begin(); it!=this->headers.end(); it++)
		std::cout << "\033[35m" << it->first << ' ' << it->second << "\033[0m" << std::endl;
	
	// std::map <std::string, std::string> :: iterator it;
 
	// it = this->headers.find("Content-Type:");
	// std::cout << it->second << std::endl;
}




// curl http://localhost:8000     			+

// curl http://localhost:8000
//    -H "Cache-Control: must-revalidate"
//    -H "Pragma: no-cache"
//    -H "Expires: 0"             			+

// curl -I http://localhost:8000  			+

// curl -I --http2 http://localhost:8000  	+

// curl -L http://localhost:8000 			+



