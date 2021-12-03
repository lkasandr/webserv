#include "Response.hpp"
#include <string.h>
#include <errno.h>
#include <string>
#include <cstdio>

Response::Response(int fd): fd(fd) 
{
	this->status_code = 200;
	this->code_description = " OK\r\n";
	this->version = "HTTP/1.1 ";
	this->server = "Server: webserv\r\n";
	this->connection = "Connection: Keep-Alive\r\n";
	this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
	this->allow_method = "";
	this->setCookie = "";
}

std::string get_date()
{
  char buff[64] = { 0 };
  time_t current_time;
  tm    *current_tm;
  time(&current_time);
  current_tm = localtime(&current_time);
  strftime(buff, sizeof buff, "%a, %d %b %Y %H:%M:%S GMT", current_tm);
  return std::string(buff);
}

void Response::check_errors(int code)
{
	switch (code)
	{
	case 204:	// No Content
		this->code_description = " No Content\r\n";
		this->content_path = "./rss/error/204.html";
		this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
		break;
	case 301: // Moved Permanently
		this->code_description = " Moved Permanently\r\n";
		this->content_path = "./rss/error/301.html";
		this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
		break;
	case 400: // Bad Request 
		this->code_description = " Bad Request\r\n";
		this->content_path = "./rss/error/400.html";
		this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
		break;
	case 404: // Not Found
		this->code_description = " Not Found\r\n";
		this->content_path = "./rss/error/404.html";
		this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
		this->connection = "Connection: Close\r\n";
		break;
	case 405: // Method Not Allowed 
		this->code_description = " Method Not Allowed\r\n";
		this->content_path = "./rss/error/405.html";
		this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
		break;
	case 413: // Payload Too Large
		this->code_description = " Payload Too Large\r\n";
		this->content_path = "./rss/error/413.html";
		this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
		this->connection = "Connection: Close\r\n";
		break;
	case 500: //  Internal Server Error
		this->code_description = " Internal Server Error\r\n";
		this->content_path = "./rss/error/500.html";
		this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
		this->connection = "Connection: Close\r\n";
		break;
	case 501: //  Not Implemented
		this->code_description = " Not Implemented\r\n";
		this->content_path = "./rss/error/501.html";
		this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
		break;
	case 505: //  HTTP Version Not Supported
		this->code_description = " HTTP Version Not Supported\r\n";
		this->content_path = "./rss/error/505.html";
		this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
		this->connection = "Connection: Close\r\n";
		break;
	default:
		break;
	}
}

void Response::make_response(Request *request, std::vector<Configuration> config)
{
	this->date = get_date() + "\r\n";
	this->status_code = request->getCode();
	if (this->status_code != 200)
		check_errors(this->status_code);
	else
	{
		check_method(config, request);
		check_errors(this->status_code);
	}

	// формируем ответ
	if (request->getMethod() == "GET")
	{
		std::ifstream	file;
		file.open(this->content_path);
		if (!file.is_open())
		{
			this->status_code = 404;
			this->code_description = " Not Found\r\n";
			this->connection = "Connection: Close\r\n";
			file.open("./rss/error/404.html");
		}
		std::stringstream content;
		content << file.rdbuf();
		std:: stringstream response;
		response << this->version << this->status_code << this->code_description
		<< this->date << this->server << this->connection << this->allow_method
		<< this->contentType << "Content-Length: " << content.str().length() << "\r\n"
		<< this->setCookie << "\r\n\r\n" << content.str();
		send(fd, response.str().c_str(), response.str().length(), 0);
		file.close();
	}
	else
	{
		std:: stringstream response;
		response << this->version << this->status_code << this->code_description
		<< this->date << this->server << this->connection << this->allow_method
		<< "\r\n\r\n" << " ";
		send(fd, response.str().c_str(), response.str().length(), 0);
	}
}

void Response::check_method(std::vector<Configuration> configs, Request *request)
{
	std::string uri_str = request->getUri();
	if (uri_str == "/ ")
		uri_str = "/home ";
	if (uri_str == "/redirect ")
		this->status_code = 301;
	std::string 	method[3] = {"GET", "POST", "DELETE"};
	int m = 0;
	while(m < 3 && request->getMethod().compare(method[m]) != 0 )
		m++;
	switch (m)
	{
	case 0:
		std::cout << "Method GET" << std::endl;
		for (std::vector<Configuration>::iterator it = configs.begin(); it != configs.end(); ++it)
		{
			if (uri_str.find(it->getlocation()) != std::string::npos)
			{	
				if (it->checkGet())
				{
					this->server = "Server: " + it->getServerName() + "\r\n";
					this->content_path = it->getIndex();
					if (this->setCookie == "")
						this->setCookie = "Set-Cookie: name=" + it->getServerName() + "\r\n";
				}	
				else
				{
					this->status_code = 405;
					this->allow_method = "Allow: " + it->getHttpMethod() + "\r\n";
				}
				break;
			}
		}
		break;
	case 1:
		std::cout << "Method POST" << std::endl;
		for (std::vector<Configuration>::iterator it = configs.begin(); it != configs.end(); ++it)
		{
			if (uri_str.find(it->getlocation()) != std::string::npos)
			{	
				if (it->checkPost())
				{
					this->server = "Server: " + it->getServerName() + "\r\n";
					if(request->getBody().size() > (size_t) it->getClientBodySize())
					{	
						this->status_code = 413;
						break;
					}
					// if(request->getBody().length() == 0 || request->getBody().size() == 0)
					// {
					// 	this->status_code = 204;
					// 	break;
					// }
					std::fstream newfile;
					newfile.open("./rss/post/test_post.txt", std::ios_base::app);
					if (!newfile.is_open())
					{
						this->status_code = 500;
						break;
					}
					newfile << request->getBody();
					this->status_code = 201;
					this->code_description = " Created.\r\n";
					this->location = "./rss/post/test_post.txt";
					this->connection = "Connection: Close\r\n";
					newfile.close();
					break;
				}	
				else
				{
					this->status_code = 405;
					this->allow_method = "Allow: " + it->getHttpMethod() + "\r\n";
				}
				break;
			}
		}
		break;
	case 2:
		std::cout << "Method DELETE" << std::endl;
		for (std::vector<Configuration>::iterator it = configs.begin(); it != configs.end(); ++it)
		{
			if (uri_str.find(it->getlocation()) != std::string::npos)
			{	
				if (it->checkDelete())
				{
					this->server = "Server: " + it->getServerName() + "\r\n";
					this->content_path = uri_str.insert(0, std::string("./rss"));
					for(size_t i = 0; i < this->content_path.length(); i++)
   					{
						if(this->content_path[i] == ' ')
						{
							this->content_path.erase(i,1);
							i--;
						}
   					}
					if (remove(this->content_path.c_str()))
        			{
						this->status_code = 500;
						std::cout << "Error in remove DELETE method." << std::endl;
					}
				}	
				else
				{
					this->status_code = 405;
					this->allow_method = "Allow: " + it->getHttpMethod() + "\r\n";
				}
				break;
			}
		}
		break;
	default:
		std::cout << "Uncknown method" << std::endl;	//501 not implemented
		this->status_code = 501;
		break;
	}	
}

Response::~Response()
{
}

// getters

int Response::getStatus_code() const
{
	return this->status_code;
}

std::string Response::getCodeDescription() const
{
	return this->code_description;
}

std::string Response::getVersion() const
{
	return this->version;
}

std::string Response::getServer() const
{
	return this->server;
}

std::string Response::getDate() const
{
	return this->date;
}

std::string Response::getAllow_method() const
{
	return this->allow_method;
}


std::ostream& operator<<(std::ostream& out, const Response& resp)
{
	out << "\033[33mResponse: \033[0m";
	out << "Status code: " << resp.getStatus_code() << " " << resp.getCodeDescription();
	out << "HTTP version: " << resp.getVersion() << std::endl;
	out << resp.getServer();
	out << "Date: " << resp.getDate();
	return (out);
}