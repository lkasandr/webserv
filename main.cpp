#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read
#include <cerrno>


#include "Configuration.hpp"
#include "Request.hpp"
#include "Socket.hpp"
#include "webserv.hpp"
#include <vector>
#include <poll.h>

int check_fd(std::vector<Socket> & sockets, int fd)
{
	for(std::vector<Socket>::iterator it = sockets.begin(); it != sockets.end(); ++it)
	{
		if (it->get_listening_socket_fd() == fd)
			return 1;
	}
	return 0;
}

void accept_connection(std::vector<Socket> & sockets, std::vector<struct pollfd> & pfds, int fd)
{
	for(std::vector<Socket>::iterator it = sockets.begin(); it != sockets.end(); ++it)
	{
		if (it->get_listening_socket_fd() == fd)
		{
			it->accept_socket();
			std::cout << it->get_accept_socket_fd();
			struct pollfd new_sock = {it->get_accept_socket_fd(), POLLIN, 0};
			pfds.push_back(new_sock);
		}
	}
}

void	main_cycle(std::vector<struct pollfd> & pfds, std::vector<Socket>	& sockets )
{
	for (size_t i = 0; i < pfds.size(); i++)
    {
       std::cout << pfds[i].revents << "pfds \n";
	    if (pfds[i].revents & POLLIN)  // Check if someone has something to read
        // {
            // If this is a server, handle connection
            if (check_fd(sockets, pfds[i].fd))
                accept_connection(sockets, pfds, pfds[i].fd);
            // Or if this is a client
            else
            {
                char *buffer = new char[25000];
				Request	req;
				int bytes_received = recv(pfds[i].fd, buffer, 25000, 0);
                // If no bytes received, then close connection
                if (bytes_received <= 0)
                {
					close(pfds[i].fd);
    				pfds.erase(pfds.begin() + i);
				}
                // Or receive data from client
                else
                {
                    req.parseRequest(buffer);
					std::cout << "The message was: " << buffer;
					delete[] buffer;
                }
            }
        // }
    }
}




int main(int argc, char **argv)
{
	const char *path = "rss/default.conf";
	if (argc == 2)
		path = argv[1];
	std::vector<Configuration>  configs;
	std::vector<Socket>			sockets;
	std::vector<struct pollfd> 	pfds;  //структура для создания очереди сокетов
	int server_count;
	// int sockets_id;
	try
	{
		server_count = read_conf(path, configs);
	}
	catch(std::string error)
	{
		std::cerr << error << std::endl;
		return 1;
	}
	for (std::vector<Configuration>::iterator it = configs.begin(); it != configs.end(); ++it)
	{
		std::cout << *it;
		// unsigned short int port = it->getPort();

		Socket	sock(it->getPort());
		std::cout << "sock.get_listening_socket_fd()" << sock.get_listening_socket_fd();
		struct pollfd pfd = {sock.get_listening_socket_fd(), POLLIN, 0};
		pfds.push_back(pfd);
		sockets.push_back(sock);
	}
	
	while (1)
	{
		// std::cout << "start web\n" ;
		struct pollfd* array = &(pfds[0]);		// из вектора структур создаем массив для системного вызова полл
		int ret = poll(array, pfds.size(), -1);
		if (ret == -1)
			std::cout << "poll error\n";

		else if ( ret == 0 ) // таймаут, событий не произошло
			std::cout << "timeout\n";
		else
			main_cycle(pfds, sockets );
	}
	// 	else
	// 	{
	// 		for (size_t i = 0; i < pfds.size(); i++)
	// 		{
	// 			if (pfds[i].revents & POLLIN) // обнаружили событие, обнулим revents чтобы можно было переиспользовать структуру
	// 			{
	// 				std::cout << pfds[i].fd;
	// 				// for(std::vector<Socket>::iterator it = sockets.begin(); it != sockets.end(); ++it)
	// 				// {
	// 					if (check_fd(sockets, pfds[i].fd))
	// 					{
	// 						it->accept_socket();
	// 						std::cout << sockets[i].get_accept_socket_fd();
	// 						struct pollfd new_sock = {sockets[i].get_accept_socket_fd(), POLLIN, 0};
	// 						pfds.push_back(new_sock);
	// 					}
	// 					else
	// 					{
	// 						char *buffer = new char[25000];
	// 						Request	req;
	// 						// std::string buffer; /* = new std::string[15000];*/
	// 						read(pfds[i].fd, buffer, 25000);
	// 						req.parseRequest(buffer);
	// 						std::cout << "The message was: " << buffer;
	// 						delete[] buffer;
	// 					}
	// 				// }
	// 				// sockets[i].accept_socket();
	// 				// std::cout << sockets[i].get_accept_socket_fd();
	// 				// struct pollfd new_sock = {sockets[i].get_accept_socket_fd(), POLLIN, 0};
    // 				// pfds.push_back(new_sock);
	// 			}
	// 		}
	// 	} 
		
	// // }



	// Socket	sock(configs[1].getPort());
	// std::cout << sock.get_accept_socket_fd() << "\n" << sock.get_listening_socket_fd() << "\n";
	// int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	// if (sockfd == -1) {
	// 	std::cout << "Failed to create socket. errno: " << errno << std::endl;
	// 	exit(EXIT_FAILURE);
	// }

	// Listen to port 9999 on any address
	// sockaddr_in sockaddr;
	// sockaddr.sin_family = AF_INET;
	// sockaddr.sin_addr.s_addr = INADDR_ANY;
	// sockaddr.sin_port = htons(8080); // htons is necessary to convert a number to
	// 								// network byte order
	// if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
	// 	std::cout << "Failed to bind to port from conf. errno: " << errno << std::endl;
	// 	exit(EXIT_FAILURE);
	// }

	// // Start listening. Hold at most 10 connections in the queue
	// if (listen(sockfd, 10) < 0) {
	// 	std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
	// 	exit(EXIT_FAILURE);
	// }

	// Grab a connection from the queue
	// auto addrlen = sizeof(sockaddr);
	// int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
	// if (connection < 0) {
	// 	std::cout << "Failed to grab connection. errno: " << errno << std::endl;
	// 	exit(EXIT_FAILURE);
	// }

	// Read from the connection
	////////////////////////////
	////////REQUEST/////////////
	///////////////////////////
	// int connection = sock.accept_socket();
	// char *buffer = new char[25000];
	// Request	req;
	// // std::string buffer; /* = new std::string[15000];*/
	// read(connection, buffer, 25000);
	// req.parseRequest(buffer);
	// // std::cout << "The message was: " << buffer;
	// delete[] buffer;


	// // Send a message to the connection
	// std::string response = "Good talking to you\n";
	// send(connection, response.c_str(), response.size(), 0);

	// Close the connections
	// close(connection);
	// close(sockfd);


	return 0;
}