#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/socket.h>
#include <netinet/in.h>

class Socket{
	private:
		int server_fd;
		struct sockaddr_in address;

	public:
		Socket(int domain, int type, int protocol, int port);
		~Socket();
};

#endif