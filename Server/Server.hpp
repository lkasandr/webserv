#ifndef SERVER_HPP
# define SERVER_HPP

#include "Socket.hpp"
#include "Configuration.hpp"
#include "webserv.hpp"
#include "Request.hpp"
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
public:
	Server(std::vector<Configuration> configs);
	~Server();

	void	main_cycle();
	void 	init_pfd(int fd);
	int 	check_fd(int fd);
	void 	accept_connection(int fd);
};



#endif