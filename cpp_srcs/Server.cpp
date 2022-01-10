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
	pos = line.find("Content-Length: ");
	if (/*(pos = line.find("Content-Length: ") + 16)*/ pos != std::string::npos)
	{
		pos = line.find("Content-Length: ") + 16;
		size_t pos_end = line.find("\n", pos);
		line = line.substr(pos, pos_end - pos);
		len = atoi(line.c_str());
	}
	return len;
}

void Server::check_ready(int fd, char *buffer, int i)
{
	// std::cout << "\033[33mBUFFER: \033[0m" << buffer << "\n";
	for (std::list<Client>::iterator it = this->clients.begin(); it != this->clients.end(); ++it)
	{
		if (it->getClientFd() == fd)
		{
			it->msg.append(buffer, i);
			if(it->msg.find("Transfer-Encoding: chunked") != std::string::npos  || it->msg.find("Content-Length") != std::string::npos\
				|| it->msg.find(" PUT ") != std::string::npos || it->msg.find(" POST ") != std::string::npos)
			{
				// std::cout << "\033[33mMSG : \033[0m" << it->msg << "\n";
				if(!it->getContentLen())
				{
					it->setContentLen(check_content_length(buffer));
					if(!it->getContentLen() && (it->msg.find("0\r\n\r\n") != std::string::npos))
					{
						it->chunk_ready = true;
						return;
					}
					else if(it->msg.length() >= (size_t)it->getContentLen() && it->msg.find("Transfer-Encoding: chunked") == std::string::npos)
					{
						// std::cout << "IT_MSG" << it->msg << "\n";
						it->chunk_ready = true;
						return;
					}
				}
				else if(it->msg.length() >= (size_t)it->getContentLen())
				{
					// std::cout << "IT_MSG" << it->msg << "\n";
					it->chunk_ready = true;
					return;
				}
			}
			else if(it->msg.find("\r\n\r\n") != std::string::npos)
			{
				// std::cout << "\033[33mMSG : \033[0m" << it->msg << "\n";
				it->chunk_ready = true;
				// std::cout << "\033[33mCHUNK_READY : \033[0m" << it->chunk_ready << "\n";
				// return;
			}
			else
				it->chunk_ready = false;
			return;
		}
	}
	return;
}

bool check_server(Request *request, std::vector<Configuration> configs, Configuration *conf)
{
	std::string host = request->getHeaders().find("Host")->second;
	for (std::vector<Configuration>::iterator it = configs.begin(); it != configs.end(); ++it)
	{
		if(it->getHost() == host)
		{
			*conf = ((*it));
			return 1;
		}
	}
	return 0;
}

bool check_client(int fd, std::list<Client> clients)
{
	for (std::list<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->getClientFd() == fd)
			return 1;
	}
	return 0;
}

void Server::communication(int fd, int i)
{
	char *buffer = new char[4096];
	memset((void *)buffer, 0, 4096);
	int message = recv(fd, buffer, 4096, 0);  // считываем входящее сообщение
	if (message <= 0)
	{
		close(fd);
		pfds.erase(pfds.begin() + i); 	// убираем fd из очереди
		throw ClientCloseConnection();
	}
	if(!check_client(fd, clients))
		clients.push_back(Client(fd));
	// std::cout << "\033[33mBUFF 153 : \033[0m" << buffer << "\n";
	check_ready(fd, buffer, message);
	for (std::list<Client>::iterator it = this->clients.begin(); it != this->clients.end(); ++it)
	{	
		if(it->chunk_ready)
		{
			it->chunk_ready = false;


			Request	request;
			// std::cout << "163\n";
			request.parseRequest(it->msg);
			// std::cout << "\033[33mRequest: \033[0m" << it->msg;
			std::cout << request;
			Configuration conf;
			if(!check_server(&request, config, &conf))
				request.setCode(400);
			Response response(fd);
			response.make_response(&request, &conf);
			std::cout << response;
			// close(fd);				///???
			// pfds.erase(pfds.begin() + i);		///???
			clients.erase(it);
			break;
			// it->~Client();
			// it->msg.clear();
		}	
	}
	delete[] buffer;
}


void	sig_handler(int signal)
{
	if(signal == SIGINT)
	{
		server->~Server();
		std::cout << "Exiting the server " << std::endl;
		exit(0);
	}
}

void	Server::main_cycle()
{
	if(!server)
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
		std::cout << "close sockets fd " << it->get_listening_socket_fd() << std::endl;
	}
	
}

const char* Server::SocketError::what(void) const throw()
{
	return "\033[31mSocket's error \033[0m";
}
