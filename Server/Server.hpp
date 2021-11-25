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

class Server
{
private:
	std::vector<Socket>			sockets;
	std::vector<struct pollfd> 	pfds;  //структура для создания очереди сокетов
	std::vector<Configuration>  config;
public:
	Server(std::vector<Configuration> configs);
	~Server();

	void	main_cycle();
	void 	init_pfd(int fd);
	int 	check_fd(int fd);
	void 	accept_connection(int fd);
	void	communication(int fd, int i);

	class ClientCloseConnection: public std::exception 
	{
	public:
		const char* what(void) const throw();
	};
};



#endif