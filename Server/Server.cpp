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

void	Server::main_cycle()
{
	struct pollfd* array = &this->pfds[0];
	int ret = poll(array, this->pfds.size(), 1000);
	if (ret == -1)
		std::cout << "poll error\n";
	// else if ( ret == 0 ) // таймаут, событий не произошло
	// 	std::cout << "timeout\n";
	else
	{
		for (size_t i = 0; i < this->pfds.size(); i++)
		{
			if (this->pfds[i].revents & POLLIN)  // проверка входящего события
			{
				if (check_fd(this->pfds[i].fd))		// проверяем в каком сокете произошло событие
					accept_connection(this->pfds[i].fd);
				else
				{
					char *buffer = new char[25000];
					int message = recv(this->pfds[i].fd, buffer, 25000, 0);  // считываем входящее сообщение
					if (message <= 0)
					{
						close(pfds[i].fd);
						pfds.erase(pfds.begin() + i);
					}
					else
					{
						Request	req;
						req.parseRequest(buffer);
						std::cout << "The message was: " << buffer;
						delete[] buffer;
						// // Send a message to the connection
						std::string response = "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Type: text/html\r\nContent-Length: 20\r\n\r\nGood talking to you\n";
						send(this->pfds[i].fd, response.c_str(), response.size(), 0);
					}
				}
			}
			if (this->pfds[i].revents & POLLOUT)
			{
				std::cout << "out\n" ;
			}

		}
	}
}


Server::~Server()
{
}

