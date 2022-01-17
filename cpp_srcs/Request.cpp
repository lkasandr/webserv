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

std::string	Request::getMethod() const
{
	return this->method;
}

std::string	Request::getQueryString() const
{
	return this->query_string;
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

std::string Request::getScriptPath() const
{
	return this->script_path;
}

std::string Request::getPort() const
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

std::map<std::string, std::string> Request::getHeaders() const
{
	return this->headers;
}

int hex_to_dec(std::string hex)
{
    int dec;

    std::stringstream ss;
    ss << hex;
    ss >> std::hex >> dec;

    return dec;
}



std::string chunk_handler(std::string body)
{
	size_t chunk_size;
	std::string new_body;
	// new_body.clear();
	while(1)
	{
		if(body == "0\r\n\r\n")
		{
			// new_body += body;
			break;
		}

		size_t crlf_pos = body.find_first_of("\r\n");
		chunk_size = hex_to_dec(body.substr(0, crlf_pos));

		if (body.substr(crlf_pos + 2).length() < chunk_size)	
		{
			// std::cout << "95 " << std::endl;
			new_body += body.substr(crlf_pos + 2, body.substr(crlf_pos + 2).length());
			// std::cout << "90NEW BODY UUU" << new_body << "UUU" << std::endl;
			body.erase(0, crlf_pos + 2 + body.substr(crlf_pos + 2).length());
		}
		else
		{
			// std::cout << "102 " << std::endl;
			new_body += body.substr(crlf_pos + 2, chunk_size);
			// std::cout << "body.substr UUU" << body.substr(crlf_pos + 2 + chunk_size + 2) << "UUU" << std::endl;
			// std::cout << "96NEW BODY UUU" << new_body << "UUU" << std::endl;
			body.erase(0, crlf_pos + 2 + chunk_size + 2);
		}
		// body.erase(0, 2);
		// const char *gup = body.c_str();
		// std::cout << "UUU" << gup << "UUU" << std::endl;
		
	}
	// std::cout << "NEW BODY UUU" << new_body << "UUU" << std::endl;
	return new_body;
}

void Request::setBody(std::string line)
{
	// const char *buff = line.c_str();
	// std::cout << "\033[35mBODY: " << line << "\033[0m" <<std::endl;
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
		// std::cout << "70 this->status_code" << this->code << std::endl;
		this->body = line;
		return ;
	}
	this->body = line;
	// const char *str = this->body.c_str();
	// std::cout << str << "\n";
	// if(this->chunked && this->body != "0\r\n\r\n")
	// 	this->body = chunk_handler(this->body);
    // if (this->body.length() == 0 || this->body.size() == 0 || this->body == "0")
    // {
	// 	this->code = 405;
	// 	std::cout << "78 this->status_code" << this->code << std::endl;
	// }
	// std::cout << "\033[35mBODY: " << this->body << "\033[0m" <<std::endl;
}

void Request::setHTTPversion(std::string line)
{
	this->http_version = line.substr(0, 8);
	if (this->http_version != "HTTP/1.1")
		this->code = 505;
	// std::cout << "HTTP VERSION: " << this->http_version << std::endl;
	// std::cout << "CODE IS " << this->code << std::endl;
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
	// std::string cgi_indicator;

	pos++;
	if (line[pos] == '/')
	{
		while(pos != line.find(" ", 1))
			pos++;
		this->uri = line.substr(1, pos - 1);
		temp = line.substr(pos + 1, line.length() - pos);
		// cgi_indicator = this->uri.substr(0, 9);
		// std:: cout << "CGI INDICATOR " << cgi_indicator << std::endl;
		if (this->uri.find("/cgi/") != std::string::npos || this->uri.find(".php") != std::string::npos	||\
			(this->method == "POST" && this->uri.find(".bla") != std::string::npos))
		{
			this->cgi = 1;
			size_t pos_cgi = 0;
			// std::cout << "116URI " << this->uri << "III\n";
			pos_cgi = this->uri.find('?');
			if ( pos_cgi != std::string::npos)
			{
				this->query_string = this->uri.substr(pos_cgi + 1);
				this->uri = this->uri.substr(0, pos_cgi);
			}

			// std::cout << "124URI " << this->uri << "III\n";
			
			// pos_cgi = this->uri.find("/cgi/");
			pos_cgi = this->uri.find_last_of("/");
			if ( pos_cgi != std::string::npos)
			{
				size_t pos_end = find_pos_end(this->uri, pos_cgi);
				this->script_path = this->uri.substr((pos_cgi + 1), (pos_end - (pos_cgi + 1)));
				// std::cout << "SCRIPT_PATH " << script_path << "III\n";
			}	
			// pos_cgi = this->uri.find('?');
			// if ( pos_cgi != std::string::npos)
			// {
			// 	this->query_string = this->uri.substr(pos_cgi + 1);
			// 	this->uri = this->uri.substr(0, pos_cgi);
			// }
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
	{
		this->code = 505;
		// std::cout << "????????????" << std::endl;
		// std::cout << "LINE: " << line << std::endl;
	}
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
			// std::cout << "PORT - " << port << "\n";
		}
		if(key == "Transfer-Encoding" && value.find("chunked") != std::string::npos)
		{
			this->chunked = true;
		}
	}
}

void		Request::parseRequest(std::string line)
{
    // std::string		line(buffer);
	// std::cout << "LINE " << line << "\n";
	std::string		temp;
	size_t prev = 0, pos = 0;
    while(pos != line.find("\n", prev))
		pos++;
	temp = line.substr(prev, pos - prev);
	parse_first_line(temp);
	line = line.substr(pos + 1, line.length());
	// std::cout << "LINE: " << line << std::endl;
	pos = 0;
	while(pos != line.length() && line[0] != '\r' && line[1] != '\n')
	{
		if (pos == line.find("\r\n"))
		{
			temp = line.substr(0, pos);
			// std::cout << "TEMP: " << temp << std::endl;
			// std::cout << "TEMP_LENGHT: " << temp.length() << std::endl;
			line = line.substr(pos + 2, line.length());
			add_headers(temp);
			// std::cout << "LINE: " << line << std::endl;
			// std::cout << "LINE_LENGHT: " << line.length() << std::endl;
			// std::cout << "POS: " << pos << std::endl;
			pos = -1;
		}
		pos++;
	}
	if (getMethod() == "POST" || getMethod() == "PUT")
	{
		// std::cout << "\nLINE in post: " << line << std::endl;
		// std::cout << "\nbuffer in post: " << line << std::endl;
		// std::cout << "LINE_LENGHT in post: " << line.length() << std::endl;
		setBody(line.substr(line.find_first_of("\n") + 1));
		// setBody(line);
	}

	
	// для вывода this->headers:
	// std::cout << "\033[35mMethod: " << getMethod() << "\033[0m" << std::endl;
	// std::cout << "\033[35mURI: " << getUri() << "\033[0m" << std::endl;
	// std::cout << "\033[35mHTTP Version: " << getHTTP_version() << "\033[0m" << std::endl;
	// std::map<std::string, std::string>::iterator it;
	// std::cout << "HEADERS: " << std::endl;
	// for (it=this->headers.begin(); it!=this->headers.end(); it++)
	// 	std::cout << "\033[35m" << it->first << ' ' << it->second << "\033[0m" << std::endl;
}

bool Request::getPostFile() const
{
	return this->post_file;
}


// curl http://localhost:8000     			+

// curl http://localhost:8000
//    -H "Cache-Control: must-revalidate"
//    -H "Pragma: no-cache"
//    -H "Expires: 0"             			+

// curl -I http://localhost:8000  			+

// curl -I --http2 http://localhost:8000  	+

// curl -L http://localhost:8000 			+



std::ostream& operator<<(std::ostream& out, const Request& request)
{
	out << "\033[33mRequest: \033[0m";
	out << request.getMethod() << " " << request.getUri() << " " << request.getHTTP_version() << std::endl;
	out << "Host: " << request.getHeaders().find("Host")->second << std::endl;
	if(request.getBody().size() < 5000)
		out << request.getBody() << std::endl;
	// if (request.getHeaders().find("Content-Length") != request.getHeaders().end())
	// 	out << "Content-Length: " << request.getHeaders().find("Content-Length")->second << std::endl;
	// if (request.getHeaders().find("Content-Type") != request.getHeaders().end())
	// 	out << "Content-Type: " << request.getHeaders().find("Content-Length")->second << std::endl;
	// if (request.getHeaders().find("Cookie") != request.getHeaders().end())
	// 	out << "Cookie: " << request.getHeaders().find("Cookie")->second << std::endl;
	
	return (out);
}