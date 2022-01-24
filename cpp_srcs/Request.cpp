#include "Request.hpp"

Request::Request()
{
	this->code = 200;
	this->cgi = 0;
	this->post_file = false;
	this->chunked = false;
}

Request::~Request()
{}

const std::string	&Request::getMethod() const
{
	return this->method;
}

const std::string	&Request::getQueryString() const
{
	return this->query_string;
}

const std::string	&Request::getUri() const
{
	return this->uri;
}

const std::string	&Request::getHTTP_version() const
{
	return this->http_version;
}

const std::string &Request::getBody() const
{
	return this->body;
}

const std::string &Request::getScriptPath() const
{
	return this->script_path;
}

const std::string &Request::getPort() const
{
	return this->port;
}

int Request::getCode() const
{
	return this->code;
}

bool Request::getCGI() const
{
	return this->cgi;
}

const std::map<std::string, std::string> &Request::getHeaders() const
{
	return this->headers;
}

std::string chunk_handler(std::string &body)
{
	std::string	chunk = body.substr(0, 100);
	std::string	newbody = "";
	int			chunk_size = strtol(chunk.c_str(), NULL, 16);
	size_t		i = 0;

	while (chunk_size)
	{
		i = body.find("\r\n", i) + 2;
		newbody += body.substr(i, chunk_size);
		i += chunk_size + 2;
		chunk = body.substr(i, 100);
		chunk_size = strtol(chunk.c_str(), NULL, 16);
	}
	return newbody;
}

void Request::setBody(std::string &line)
{
	if(this->chunked && line != "0\r\n\r\n")
		line = chunk_handler(line);
	if(this->method == "POST" && this->uri.find("/post_body") != std::string::npos)
	{
		if(line.length() > 100)
			this->code = 413;
		else
			this->code = 200;
	}
	else if (line.length() == 0 || (line == "0\r\n\r\n"))
	{
		this->code = 405;
		this->body = line;
		return ;
	}
	this->body = line;
}

void Request::setHTTPversion(const std::string &line)
{
	this->http_version = line.substr(0, 8);
	if (this->http_version != "HTTP/1.1")
		this->code = 505;
}

size_t find_pos_end(std::string &uri, size_t pos_cgi)
{
	size_t pos1 = uri.find(" ", pos_cgi);
	size_t pos2 = uri.find("?", pos_cgi);
	return ((pos1 < pos2) ? pos1 : pos2);
}

std::string Request::setURI(std::string line)
{
	size_t pos = 0;
	std::string temp;

	pos++;
	if (line[pos] == '/')
	{
		while(pos != line.find(" ", 1))
			pos++;
		this->uri = line.substr(1, pos - 1);
		temp = line.substr(pos + 1, line.length() - pos);
		if (this->uri.find("/cgi/") != std::string::npos || this->uri.find(".php") != std::string::npos	||\
			(this->method == "POST" && this->uri.find(".bla") != std::string::npos))
		{
			this->cgi = 1;
			size_t pos_cgi = 0;
			pos_cgi = this->uri.find('?');
			if ( pos_cgi != std::string::npos)
			{
				this->query_string = this->uri.substr(pos_cgi + 1);
				this->uri = this->uri.substr(0, pos_cgi);
			}
			pos_cgi = this->uri.find_last_of("/");
			if ( pos_cgi != std::string::npos)
			{
				size_t pos_end = find_pos_end(this->uri, pos_cgi);
				this->script_path = this->uri.substr((pos_cgi + 1), (pos_end - (pos_cgi + 1)));
			}	
		}
	}
	return temp;
}

void Request::setCode(int code)
{
	this->code = code;
}

std::string		Request::setMethod(std::string line)
{
	u_long i = 0;
	size_t pos = 0;
	std::string temp;
	std::string	available_methods[4] = {"GET", "POST", "DELETE", "PUT"};

	while(pos != line.find(" ", 0) && (pos != line.length()))
		pos++;
	this->method = line.substr(0, pos);
	for(i = 0; i < available_methods->size(); i++)
	{
		if (this->method == available_methods[i])
			break;
	}
	if (i == 4)
		this->code = 405;
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
		if(key == "Content-Type" && value.find("multipart/form-data") != std::string::npos)
		{
			this->post_file = true;
			size_t pos1 = value.find("boundary=") + 9;
			size_t pos2 = value.find("\r\n");
			this->boundary = value.substr(pos1, pos2 - pos1);
		}
		if(key == "Host")
		{
			size_t pos1 = value.find(":") + 1;
			size_t pos2 = value.find("\r\n");
			this->port = value.substr(pos1, pos2 - pos1);
		}
		if(key == "Transfer-Encoding" && value.find("chunked") != std::string::npos)
		{
			this->chunked = true;
		}
	}
}

void		Request::parseRequest(std::string line)
{
	std::string		temp;
	size_t prev = 0, pos = 0;
    while(pos != line.find("\n", prev))
		pos++;
	temp = line.substr(prev, pos - prev);
	parse_first_line(temp);
	line = line.substr(pos + 1, line.length());
	pos = 0;
	while(pos != line.length() && line[0] != '\r' && line[1] != '\n')
	{
		if (pos == line.find("\r\n"))
		{
			temp = line.substr(0, pos);
			line = line.substr(pos + 2, line.length());
			add_headers(temp);
			pos = -1;
		}
		pos++;
	}
	if (getMethod() == "POST" || getMethod() == "PUT")
	{
		std::string temp_line = line.substr(line.find_first_of("\n") + 1);
		setBody(temp_line);
	}
}

bool Request::getPostFile() const
{
	return this->post_file;
}

std::ostream& operator<<(std::ostream& out, const Request& request)
{
	out << "\033[33mRequest: \033[0m";
	out << request.getMethod() << " " << request.getUri() << " " << request.getHTTP_version() << std::endl;
	out << "Host: " << request.getHeaders().find("Host")->second << std::endl;
	if(request.getBody().size() < 1000)
		out << request.getBody() << std::endl;
	return (out);
}