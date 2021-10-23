#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read
#include <cerrno>


#include "Configuration.hpp"

void read_conf(char *conf_path, Configuration &conf)
{
	size_t pos_beg;
	size_t pos_end;
	std::string	line;
	// std::string	filename;
	std::ifstream	file;
	// filename = conf_path;
	file.open(conf_path);
	
	// std::cout << conf.getHost() << std::endl;
	// conf.setHost("drt");
	// file.open(conf_path.c_str());
	// file.open(conf_path);
	if (!file.is_open())
		throw std::string("\033[31mThe configuration file could not be opened\033[0m");
	else if (file.peek() == EOF)
		throw std::string("\033[31mThe configuration file is empty\033[0m");
	while (!file.eof())
	{
		std::getline(file, line);
		pos_beg = 0;
		pos_end = 0;
		while (1)
		{
			if (line.find("Host ", pos_beg) != std::string::npos)
			{
				pos_beg = line.find("Host ", pos_beg) + 5;
				pos_end = line.find(";", pos_beg);
				conf.setHost(line.substr(pos_beg, pos_end - pos_beg));
			}
			else if (line.find("Port ", pos_beg) != std::string::npos)
			{
				pos_beg = line.find("Port") + 5;
				pos_end = line.find(";", pos_beg);
				conf.setPort(line.substr(pos_beg, pos_end - pos_beg));
			}
			else if (line.find("server_name ", pos_beg) != std::string::npos)
			{
				pos_beg = line.find("server_name ") + 12;
				pos_end = line.find(";", pos_beg);
				conf.setServerName(line.substr(pos_beg, pos_end - pos_beg));
			}
			else if (line.find("default_error_pages ", pos_beg) != std::string::npos)
			{
				pos_beg = line.find("default_error_pages ") + 20;
				pos_end = line.find(";", pos_beg);
				conf.setDefaultErrorPages(line.substr(pos_beg, pos_end - pos_beg));
			}
			else if (line.find("client_body_size ", pos_beg) != std::string::npos)
			{
				pos_beg = line.find("client_body_size ") + 17;
				pos_end = line.find(";", pos_beg);
				conf.setClientBodySize(line.substr(pos_beg, pos_end - pos_beg));
			}
			else
				break;
				
		}
	}
	file.close();
}


int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "\033[31mMissing configuration file!\033[0m" << std::endl;
		return (1);
	}
	Configuration conf;
	try
	{
		read_conf(argv[1], conf);
	}
	catch(std::string error)
	{
		std::cerr << error << '\n';
		return 1;
	}
	std::cout << conf;


	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		std::cout << "Failed to create socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	// Listen to port 9999 on any address
	sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	sockaddr.sin_port = htons(8080); // htons is necessary to convert a number to
									// network byte order
	if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
		std::cout << "Failed to bind to port from conf. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	// Start listening. Hold at most 10 connections in the queue
	if (listen(sockfd, 10) < 0) {
		std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	// Grab a connection from the queue
	auto addrlen = sizeof(sockaddr);
	int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
	if (connection < 0) {
		std::cout << "Failed to grab connection. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	// Read from the connection
	char buffer[150000];
	read(connection, buffer, 150000);
	std::cout << "The message was: " << buffer;

	// Send a message to the connection
	std::string response = "Good talking to you\n";
	send(connection, response.c_str(), response.size(), 0);

	// Close the connections
	close(connection);
	close(sockfd);


	return 0;
}