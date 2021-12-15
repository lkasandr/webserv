#include "Server.hpp"
#include <cstring>
#include <string.h>
#include <stdio.h>

Server	*server;

Server::Server(std::vector<Configuration> configs)
{
	this->config = configs;
	for (std::vector<Configuration>::iterator it = configs.begin(); it != configs.end(); ++it)
	{
		std::cout << *it;
		try{
			Socket	sock(it->getPort());
			if (sock.setting_socket() == -1)
				throw SocketError();
			this->sockets.push_back(sock);
			init_pfd(sock.get_listening_socket_fd());
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
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

			//найти конфиг в котором этот сокет создан, по порту
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

int check_content_length(char *buffer)
{
	std::string line(buffer);
	int len = 0;
	size_t pos;
	if ((pos = line.find("Content-Length: ") + 16) != std::string::npos)
	{
		size_t pos_end = line.find("\n", pos);
		line = line.substr(pos, pos_end - pos);
		std::cout << "LINE 68 " << line << "\n";
		len = atoi(line.c_str());
	}
	std::cout << "LEN 69 " << len << "\n";
	return len;
}

std::string check_file_end(std::string bound)
{
	// std::string bound(buffer);
	size_t pos1 = bound.find("boundary=") + 9;
	size_t pos2 = bound.find("\r\n", pos1);
	std::string boundary = bound.substr(pos1, pos2 - pos1);
	boundary = boundary + "--";
	std::cout << "BOUND " << boundary << std::endl;
	return boundary;
}

bool Server::check_client(int fd, char *buffer, int i)
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
		// else if (it->getFileFd() == fd)
		// {
		// 	size_t read = 0;
		// 	char   buffer[100000];

		// 	read = fread(buffer, sizeof(char), 100000, it->getStream());
		// 	if (ferror(it->getStream()))
		// 		std::cout << "File read error" << std::endl;
		// 	it->msg.append(buffer, read);
		// 	if (feof(it->getStream()))
		// 		it->chunk_ready = true;
		// 	else 
		// 		it->chunk_ready = false;
		// 	return 1;
		// }
	}
	return 0;
}

void Server::communication(int fd, int i)
{
	char *buffer = new char[8000000];
	memset((void *)buffer, 0, 8000000);
	int message = recv(fd, buffer, 800000, 0);  // считываем входящее сообщение
	std::cout << "BUFFER " << buffer << "\n";
	if (message <= 0)
	{
		close(fd);
		pfds.erase(pfds.begin() + i); 	// убираем fd из очереди
		throw ClientCloseConnection();
	}
	if(!check_client(fd, buffer, i))
	{
		if (strstr(buffer, "Transfer-Encoding: chunked") != 0 || strstr(buffer, "Content-Type: multipart/form-data") != 0)
		{
			clients.push_back(Client(fd));
			check_client(fd, buffer, i);
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
			request.parseRequest(buf);
			std::cout << "\033[33mRequest: \033[0m" << buf;
			Response response(fd);
			response.make_response(&request, config);
			close(fd);				///???
			pfds.erase(pfds.begin() + i);		///???
			// clients.erase(it);
			it->msg.clear();
			std::cout << response;
			delete [] buf;
		}	
	}
	delete[] buffer;
}


void	sig_handler(int signal)
{
	server->~Server();
	std::cout << "Exiting the server " << std::endl;
}

void	Server::main_cycle()
{
	server = this;
	signal(SIGINT, sig_handler);	
	try
	{
		struct pollfd* array = &this->pfds[0];
		int ret = poll(array, this->pfds.size(), 1000);
		if (ret == -1)
		{
			std::cerr << "Poll close\n";
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
						communication(this->pfds[i].fd, i);	 // добавить какой конфиг
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
		std::cout << "close sockets fd " << it->get_listening_socket_fd() << std::endl;
	}
	
}

const char* Server::SocketError::what(void) const throw()
{
	return "\033[31mSocket's error \033[0m";
}
