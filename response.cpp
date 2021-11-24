#include "Server.hpp"
#include "Configuration.hpp"
#include "Request.hpp"
#include "Socket.hpp"
#include "webserv.hpp"
#include <vector>
#include <poll.h>

void get_method(int fd)
{
	std::ifstream	file;
	file.open("./rss/html/index.html");
	if (!file.is_open())
		std::cout << "error html" << std::endl;
	std::stringstream content;
	content << file.rdbuf();
	// std::cout << content.str();
	std:: stringstream response;
	response << "HTTP/1.1 200 OK\r\n" << "Version: HTTP/1.1\r\n"
	<< "Content-Type: text/html; charset=utf-8\r\n"
	<< "Content-Length: " << content.str().length()
	<< "\r\n\r\n"
	<< content.str();

	// std::string response = "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Type: text/html\r\nContent-Length: 20\r\n" + content.str() + "\r\n\r\n";
	send(fd, response.str().c_str(), response.str().length(), 0);
	// send(this->pfds[i].fd, content.str().c_str(), content.str().size(), 0);
	file.close();
}

void response(int fd)
{
	char *buffer = new char[25000];
	int message = recv(fd, buffer, 25000, 0);  // считываем входящее сообщение
	if (message <= 0)
	{
		close(pfds[i].fd);
		pfds.erase(pfds.begin() + i);
	}
	Request	request;
	request.parseRequest(buffer);
	std::cout << "The message was: " << buffer;
	delete[] buffer;

	// // // Send a message to the connection
	std::string 	method[3] = {"GET", "POST", "DELETE"};
	int i = 0;
	while(i < 3 && req.getMethod().compare(method[i]) != 0 )
		i++;
	switch (i)
	{
	case 0:
		std::cout << "Method GET" << std::endl;
		get_method(fd);
		break;
	case 1:
		std::cout << "Method POST" << std::endl;
		break;
	case 2:
		std::cout << "Method DELETE" << std::endl;
		break;
	
	default:
		std::cout << "Uncknown method" << std::endl;
		break;
	}
	
	
	// if (req.getMethod() == "GET")
	// {
	// 	std::ifstream	file;
	// 	file.open("./rss/html/index.html");
	// 	if (!file.is_open())
	// 		std::cout << "error html" << std::endl;
	// 	std::stringstream content;
	// 	content << file.rdbuf();
	// 	// std::cout << content.str();
	// 	std:: stringstream response;
	// 	response << "HTTP/1.1 200 OK\r\n" << "Version: HTTP/1.1\r\n"
	// 	<< "Content-Type: text/html; charset=utf-8\r\n"
	// 	<< "Content-Length: " << content.str().length()
	// 	<< "\r\n\r\n"
	// 	<< content.str();

	// 	// std::string response = "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Type: text/html\r\nContent-Length: 20\r\n" + content.str() + "\r\n\r\n";
	// 	send(fd, response.str().c_str(), response.str().length(), 0);
	// 	// send(this->pfds[i].fd, content.str().c_str(), content.str().size(), 0);
	// 	file.close();
	// }
	
}