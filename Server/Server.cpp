#include "Server.hpp"

void Server::init_pfd(int fd)
{
	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	this->pfds.push_back(pfd);
	std::cout << pfds[0].revents << "10pfds-0 \n";
}

Server::Server(std::vector<Configuration> configs)
{
	for (std::vector<Configuration>::iterator it = configs.begin(); it != configs.end(); ++it)
	{
		std::cout << *it;
		// unsigned short int port = it->getPort();

		// Socket	sock(it->getPort());
		// std::cout << "sock.get_listening_socket_fd()" << sock.get_listening_socket_fd();
		this->sockets.push_back(Socket(it->getPort()));
		// struct pollfd pfd = {sockets.back().get_listening_socket_fd(), POLLIN, 0};
		// this->pfds.push_back(pfd);
		init_pfd(sockets.back().get_listening_socket_fd());
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
			std::cout << it->get_accept_socket_fd();
			// struct pollfd new_sock = {it->get_accept_socket_fd(), POLLIN, 0};
			// pfds.push_back(new_sock);
		}
	}
}

void	Server::main_cycle()
{
	// struct pollfd* array = &this->pfds[0];
	// std::cout << pfds[0].revents << "56pfds-0 \n";
	// std::cout << array[0].revents << "array-0 58\n";
	// int ret = poll(array, this->pfds.size(), 1000);

	struct pollfd* pfds_array = &(pfds[0]);
	std::cout << pfds[0].revents << "62pfds-0 \n";
    int poll_count = poll(pfds_array, pfds.size(), -1);

	if (poll_count == -1)
		std::cout << "poll error\n";
	std::cout << pfds_array[0].revents << "array-0 67\n";
	std::cout << pfds[0].revents << "pfds-0 68\n";
	// else if ( ret == 0 ) // таймаут, событий не произошло
	// 	std::cout << "timeout\n";
	// else
	// {
	for (size_t i = 0; i < this->pfds.size(); i++)
		{
			std::cout << pfds[i].revents << "pfds \n";
			// if (pfds[i].revents != 0) {
			// printf("  fd=%d; events: %s%s%s\n", pfds[i].fd,
			// 		(pfds[i].revents & POLLIN)  ? "POLLIN "  : "",
			// 		(pfds[i].revents & POLLHUP) ? "POLLHUP " : "",
			// 		(pfds[i].revents & POLLERR) ? "POLLERR " : "");}
			
			
			if (this->pfds[i].revents & POLLIN)  // проверка входящего события
			{
				if (check_fd(this->pfds[i].fd))		// проверяем в каком сокете произошло событие
					accept_connection(this->pfds[i].fd);
				else
				{
					char *buffer = new char[25000];
					int message = recv(this->pfds[i].fd, buffer, 25000, 0); // считываем входящее сообщение
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
					}
				}
			}
			if (this->pfds[i].revents & POLLOUT)
			{
				std::cout << "out\n" ;
			}

		}
	// }
}




Server::~Server()
{
}

