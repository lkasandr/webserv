#include "Server.hpp"

Server::Server(std::vector<Configuration> configs)
{
	this->config = configs;
	for (std::vector<Configuration>::iterator it = configs.begin(); it != configs.end(); ++it)
	{
		std::cout << *it;
		Socket	sock(it->getPort());
		this->sockets.push_back(sock);
		init_pfd(sock.get_listening_socket_fd());
	}
}

void Server::init_pfd(int fd)
{
	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	this->pfds.push_back(pfd);
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

void Server::communication(int fd, int i)
{
	char *buffer = new char[25000];
	int message = recv(fd, buffer, 25000, 0);  // считываем входящее сообщение
	if (message <= 0)
	{
		close(fd);
		pfds.erase(pfds.begin() + i); 	// убираем fd из очереди
		throw ClientCloseConnection();
	}
	Request	request;
	request.parseRequest(buffer);
	std::cout << "\033[33mRequest: \033[0m" << buffer;
	delete[] buffer;
	Response response(fd);
	response.make_response(&request, config);
	std::cout << response;
}

void	Server::main_cycle()
{
	try
	{
		struct pollfd* array = &this->pfds[0];
		int ret = poll(array, this->pfds.size(), 1000);
		if (ret == -1)
		{
			std::cerr << "Poll error\n";
			exit(1);
		}
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
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}

const char* Server::ClientCloseConnection::what(void) const throw()
{
	return "Client close connection";
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
