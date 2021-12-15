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
	int		file_fd;
	FILE*	stream;
	// Request	request;
public:
	Client(int fd);
	Client(int file_fd, bool file );
	~Client();

	int 	getClientFd() const;
	void	setContentLen(int len);
	int		getContentLen() const;
	int		getFileFd() const;
	FILE*	getStream();
	std::string msg;
	bool	chunk_ready;
	bool	rec_file;
	std::string bound;
};



#endif