#include "Server.hpp"
#include <cstring>

Server::Server(std::vector<Configuration> configs)
{
	this->config = configs;
	for (std::vector<Configuration>::iterator it = configs.begin(); it != configs.end(); ++it)
	{
		std::cout << *it;
		Socket	sock(it->getPort());
		try{
		if (sock.setting_socket() == -1)
			throw SocketError();
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
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

bool Server::check_client(int fd, char *buffer)
{
	for (std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end(); ++it)
	{
		if (it->getClientFd() == fd)
		{
			it->msg.append(buffer);
			if (it->msg.find("0\r\n\r\n"))
				it->chunk_ready = true;
			else 
				it->chunk_ready = false;
			return 1;
		}
	}
	return 0;
}

void Server::communication(int fd, int i)
{
	char *buffer = new char[25000];
	memset((void *)buffer, 0, 25000);
	int message = recv(fd, buffer, 25000, 0);  // считываем входящее сообщение
	if (message <= 0)
	{
		close(fd);
		pfds.erase(pfds.begin() + i); 	// убираем fd из очереди
		throw ClientCloseConnection();
	}
	if(!check_client(fd, buffer))
	{
		if (strstr(buffer, "Transfer-Encoding: chunked") != 0)
		{
			clients.push_back(Client(fd));
			check_client(fd, buffer);
		}
		else
		{
			Request	request;
			request.parseRequest(buffer);
			std::cout << "\033[33mRequest: \033[0m" << buffer;
			Response response(fd);
			response.make_response(&request, config);
			close(fd);				///???
			pfds.erase(pfds.begin() + i);		///???
			std::cout << response;
		}
	}
	for (std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end(); ++it)
	{	
		if(it->chunk_ready)
		{
			it->chunk_ready = false;
			char *buf = strdup(it->msg.c_str());		
			Request	request;
			std::cout << "\033[33mBUF: " << buf << "\033[0m";
			request.parseRequest(buf);
			std::cout << "\033[33mCHUNK Request: \033[0m" << buf;
			Response response(fd);
			response.make_response(&request, config);
			close(fd);				///???
			pfds.erase(pfds.begin() + i);		///???
			it->msg.clear();
			std::cout << response;
			delete [] buf;
		}	
	}
	delete[] buffer;
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

const char* Server::SocketError::what(void) const throw()
{
	return "\033[31mSocket's error \033[0m";
}
