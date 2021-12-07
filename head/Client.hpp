#ifndef CLIENT_HPP
# define CLIENT_HPP


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

class Request;

class Client
{
private:
	int		fd;
	
	// Request	request;
public:
	Client(int fd);
	~Client();

	int getClientFd() const;
	std::string msg;
	bool	chunk_ready;
};



#endif