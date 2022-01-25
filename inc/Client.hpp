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
	int		content_len;
	bool	need_body;
public:
	Client(int fd, char *buffer);
	~Client();

	int 		getClientFd() const;
	void		setContentLen(int len);
	int			getContentLen() const;
	bool		check_need_body() const;
	bool		chunk_ready;
	std::string msg;
	bool		chunked;
};

#endif