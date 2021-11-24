#include "Server.hpp"

void Server::init_pfd(int fd)
{
	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	this->pfds.push_back(pfd);
}

Server::Server(std::vector<Configuration> configs)
{
	for (std::vector<Configuration>::iterator it = configs.begin(); it != configs.end(); ++it)
	{
		std::cout << *it;
		Socket	sock(it->getPort());
		this->sockets.push_back(sock);
		init_pfd(sock.get_listening_socket_fd());
	}
}


int Server::check_fd(int fd)
{
	for(std::vector<Socket>::iterator it = this->sockets.begin(); it != this->sockets.end(); ++it)
	{
		if (it->get_listening_socket_fd() == fd)
			return 1;
	}
	return 0;
}

void Server::accept_connection(int fd)
{
	for(std::vector<Socket>::iterator it = this->sockets.begin(); it != this->sockets.end(); ++it)
	{
		if (it->get_listening_socket_fd() == fd)
		{
			it->accept_socket();
			struct pollfd renew = {it->get_accept_socket_fd(), POLLIN, 0};
			pfds.push_back(renew);
		}
	}
}


void get_method(int fd)
{
	std::ifstream	file;
	file.open("./rss/html/main.html");
	if (!file.is_open())
		std::cout << "error html" << std::endl;
	std::stringstream content;
	content << file.rdbuf();
	std:: stringstream response;
	response << "HTTP/1.1 200 OK\r\n" << "Version: HTTP/1.1\r\n"
	<< "Content-Type: text/html; charset=utf-8\r\n"
	<< "Content-Length: " << content.str().length()
	<< "\r\n\r\n"
	<< content.str();
	send(fd, response.str().c_str(), response.str().length(), 0);
	file.close();
}

int check_method(std::string method, int port)
{
	for (std::vector<Configuration>::iterator it = configs.begin(); it != configs.end(); ++it)
	{
		if (it->getPort() == port)
			return (it->checkGet())
	}
	return 0;
}

void Server::communication(int fd, int i)
{
	char *buffer = new char[25000];
	int message = recv(fd, buffer, 25000, 0);  // считываем входящее сообщение
	if (message <= 0)
	{
		close(fd);
		pfds.erase(pfds.begin() + i); 	// убираем fd из очереди
		std::cout << "Client close connection" << std::endl;
	}
	Request	request;
	request.parseRequest(buffer);
	std::cout << "The message was: " << buffer;
	delete[] buffer;

	// // // Send a message to the connection
	std::string 	method[3] = {"GET", "POST", "DELETE"};
	int m = 0;
	while(m < 3 && request.getMethod().compare(method[m]) != 0 )
		m++;
	switch (m)
	{
	case 0:
		std::cout << "Method GET" << std::endl;
		for (std::vector<Configuration>::iterator it = configs.begin(); it != configs.end(); ++it)
		{
			if (it->getPort() == request.getPort())
			{	
				if (it->checkGet())
					get_method(fd);
			}
		}
		break;
	case 1:
		std::cout << "Method POST" << std::endl;
		for (std::vector<Configuration>::iterator it = configs.begin(); it != configs.end(); ++it)
		{
			if (it->getPort() == request.getPort())
			{	
				if (it->checkPost())
					post_method(fd);
			}
		}
		break;
	case 2:
		std::cout << "Method DELETE" << std::endl;
		for (std::vector<Configuration>::iterator it = configs.begin(); it != configs.end(); ++it)
		{
			if (it->getPort() == request.getPort())
			{	
				if (it->checkDelete())
					delete_method(fd);
			}
		}
		break;
	default:
		std::cout << "Uncknown method" << std::endl;
		break;
	}	
}

void	Server::main_cycle()
{
	struct pollfd* array = &this->pfds[0];
	int ret = poll(array, this->pfds.size(), 1000);
	if (ret == -1)
		std::cout << "poll error\n";
	else
	{
		for (size_t i = 0; i < this->pfds.size(); i++)
		{
			if (this->pfds[i].revents & POLLIN)  // проверка входящего события
			{
				if (check_fd(this->pfds[i].fd))		// проверяем в каком сокете произошло событие
					accept_connection(this->pfds[i].fd);
				else
					communication(this->pfds[i].fd, i);	
			}
		}
	}
}


Server::~Server()
{
	for (std::vector<Socket>::iterator it = sockets.begin(); it != sockets.end(); ++it)
	{
		close(it->get_accept_socket_fd());
		close(it->get_listening_socket_fd());
		std::cout << "close sockets fd" << std::endl;
	}
}
