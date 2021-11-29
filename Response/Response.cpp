#include "Response.hpp"
#include <string.h>
#include <errno.h>

Response::Response(int fd): fd(fd) 
{
	this->status_code = 200;
	this->code_description = " OK\r\n";
	this->version = "HTTP/1.1 ";
	this->server = "Server: webserv\r\n";
	this->connection = "Connection: Keep-Alive\r\n";
	this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
	this->allow_method = "";
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
		this->content_path = "./rss/error/error.html";
		this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
		break;
	case 301: // Moved Permanently
		this->code_description = " Moved Permanently\r\n";
		this->content_path = "./rss/error/error.html";
		this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
		break;
	case 400: // Bad Request 
		this->code_description = " Bad Request\r\n";
		this->content_path = "./rss/error/error.html";
		this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
		break;
	case 404: // Not Found
		this->code_description = " Not Found\r\n";
		this->content_path = "./rss/error/error.html";
		this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
		this->connection = "Connection: Close\r\n";
		break;
	case 405: // Method Not Allowed 
		this->code_description = " Method Not Allowed\r\n";
		this->content_path = "./rss/error/error.html";
		this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
		break;
	case 413: // Payload Too Large
		this->code_description = " Payload Too Large\r\n";
		this->content_path = "./rss/error/error.html";
		this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
		break;
	case 500: //  Internal Server Error
		this->code_description = " Internal Server Error\r\n";
		this->content_path = "./rss/error/error.html";
		this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
		this->connection = "Connection: Close\r\n";
		break;
	case 501: //  Not Implemented
		this->code_description = " Not Implemented\r\n";
		this->content_path = "./rss/error/error.html";
		this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
		break;
	case 505: //  HTTP Version Not Supported
		this->code_description = " HTTP Version Not Supported\r\n";
		this->content_path = "./rss/error/error.html";
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
	// if (this->status_code != 200)
	// 	check_errors(this->status_code);
	// else
	// {
		check_method(config, request);
		check_errors(this->status_code);
	// }

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
			file.open("./rss/error/error.html");
		}
		std::stringstream content;
		content << file.rdbuf();
		std:: stringstream response;
		response << this->version << this->status_code << this->code_description
		<< this->date << this->server << this->connection << this->allow_method
		<< this->contentType << "Content-Length: " << content.str().length() 
		<< "\r\n\r\n" << content.str();
		send(fd, response.str().c_str(), response.str().length(), 0);
		file.close();
	}
	else
	{
		std:: stringstream response;
		response << this->version << this->status_code << this->code_description
		<< this->date << this->server << this->connection << this->allow_method
		<< "\r\n\r\n";
		send(fd, response.str().c_str(), response.str().length(), 0);
	}
}

// void	send_icon(int fd)
// {
// 	std::vector<char> 	m_fav_icon_buffer;
// 	std::ifstream f_icon_file;
// 	// fs::path path_icon_file = fs::current_path() / "icons" / "favicon.png";
// 	f_icon_file.open("./rss/html/icons8.png", std::ios::binary | std::ios::ate);

// 	if(!f_icon_file.is_open())
// 	{
// 		std::cerr << "favicon file did not opened! path - " << "./rss/html/favicon.ico" << std::endl;
// 	}

// 	std::streamsize size = f_icon_file.tellg();
// 	m_fav_icon_buffer.resize(size);
// 	f_icon_file.seekg(0, std::ios::beg);
// 	if(!f_icon_file.read(m_fav_icon_buffer.data(), size))
// 	{
// 		std::cerr << "favicon read error!" << std::endl;
// 	}
// 	f_icon_file.close();
// 	std::stringstream response;
// 	response << "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Type: image/png\r\nContent-Length: " << m_fav_icon_buffer.size() << "\r\n\r\n"
// 	<< m_fav_icon_buffer.data();
	
// 	send(fd, response.str().c_str(), response.str().size(), 0);
// // 	// std::ifstream 		file;
// // 	// file.open("./rss/html/favicon.ico");

// // 	// // favicon_buffer.data()
// // 	// std::stringstream content;
// // 	// content << file.rdbuf();
// // 	// std::stringstream response;
// //     // response << "HTTP/1.1 200 OK\r\nConnection: Keep-alive\r\nContent-Type: image/png\r\nContent-Length: " << content.str().size() << "\r\n\r\n" << content.str();
// // 	send(fd, response.str().c_str(), response.str().length(), 0);
// }

// void	Response::method_post(std::vector<Configuration> configs, Request *request)
// {
// 	std::fstream postFile;

// 	postFile.open(uri.c_str(), std::ios::app);

// 	if (!postFile.is_open())
// 		throw std::string "POST error, make file\n"
	
// 	postFile << _req->getBody().getBody();
// }


void Response::check_method(std::vector<Configuration> configs, Request *request)
{
	std::string uri_str = request->getUri();
	if (uri_str == "/ ")
		uri_str = "/home ";
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
			// if (uri_str.find("favicon"))
			// {
			// 	send_icon(this->fd);
			// 	break;
			// }
			if (uri_str.find(it->getlocation()) != std::string::npos)
			{	
				if (it->checkGet())
				{
					this->server = "Server: " + it->getServerName() + "\r\n";
					this->content_path = it->getIndex();
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
					this->content_path = it->getIndex();
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
					this->content_path = "./rss" + uri_str;
					std::cout << this->content_path.c_str() << std::endl;
					 if (remove(this->content_path.c_str()))
        			{
						// webserv/rss/delete/file.c
						// throw std::string ("Error in remove DELETE method.");
						std::cout << "Error in remove DELETE method." ;
						std::cerr << errno << strerror(errno);
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


std::ostream& operator<<(std::ostream& out, const Response& resp)
{
	out << "\nResponse:\n";
	// out << "Status code: " << resp.status_code << resp.code_description << std::endl;
	// out << "Port: " << config.getPort() << std::endl;
	// out << "server_name: " << config.getServerName() << std::endl;
	// out << "default_error_pages: " << config.getDefaultErrorPages() << std::endl;
	// out << "client body size: " << config.getClientBodySize() << std::endl;
	// out << "HTTP methods: " << config.getHttpMethod() << std::endl;
	// out << "Location: " << config.getlocation() << std::endl;
	// out << "Index: " << config.getIndex() << std::endl;
	return (out);
}