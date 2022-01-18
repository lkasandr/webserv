#ifndef SERVER_HPP
# define SERVER_HPP


#include "Socket.hpp"
#include "Configuration.hpp"
#include "webserv.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include <vector>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fstream>
#include <sstream>
#include "Client.hpp"
#include <signal.h>
#include "CGI.hpp"
#include <list>
#include <cctype>

class Client;
class Response;

class Server
{
private:
	std::vector<Socket>			sockets;
	std::vector<struct pollfd> 	pfds; 
	std::vector<Configuration>  config;
	std::list<Client>			clients;
	char *						buffer;
public:
	Server(std::vector<Configuration> &configs);
	~Server();

	void	main_cycle();
	void 	init_pfd(int fd);
	int 	check_fd(int fd);
	void 	accept_connection(int fd);
	void	communication(int fd, int i);
	void 	check_ready(int fd, char* buffer, int i);


	class ClientCloseConnection: public std::exception 
	{
	public:
		const char* what(void) const throw();
	};

	class SocketError: public std::exception 
	{
	public:
		const char* what(void) const throw();
	};
};



#endif