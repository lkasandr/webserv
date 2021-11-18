#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read
#include <cerrno>

#include "Server.hpp"
#include "Configuration.hpp"
#include "Request.hpp"
#include "Socket.hpp"
#include "webserv.hpp"
#include <vector>
#include <poll.h>

int main(int argc, char **argv)
{
	const char *path = "rss/default.conf";
	if (argc == 2)
		path = argv[1];
	std::vector<Configuration>  configs;
	int server_count;
	try
	{
		server_count = read_conf(path, configs);
	}
	catch(std::string error)
	{
		std::cerr << error << std::endl;
		return 1;
	}

	Server server(configs);
	while (1)
	{
		server.main_cycle();
	}
	
	// // Send a message to the connection
	// std::string response = "Good talking to you\n";
	// send(connection, response.c_str(), response.size(), 0);

	// Close the connections
	// close(connection);
	// close(sockfd);


	return 0;
}