#include "Webserv.hpp"

// void read_conf(char *conf_path, Configuration &conf)
// {
// 	size_t pos_beg;
// 	size_t pos_end;
// 	std::string	line;
// 	// std::string	filename;
// 	std::ifstream	file;
// 	// filename = conf_path;
// 	file.open(conf_path);
	
// 	// std::cout << conf.getHost() << std::endl;
// 	// conf.setHost("drt");
// 	// file.open(conf_path.c_str());
// 	// file.open(conf_path);
// 	if (!file.is_open())
// 		throw std::string("\033[31mThe configuration file could not be opened\033[0m");
// 	else if (file.peek() == EOF)
// 		throw std::string("\033[31mThe configuration file is empty\033[0m");
// 	while (!file.eof())
// 	{
// 		std::getline(file, line);
// 		pos_beg = 0;
// 		pos_end = 0;
// 		while (1)
// 		{
// 			if (line.find("Host ", pos_beg) != std::string::npos)
// 			{
// 				pos_beg = line.find("Host ", pos_beg) + 5;
// 				pos_end = line.find(";", pos_beg);
// 				conf.setHost(line.substr(pos_beg, pos_end - pos_beg));
// 			}
// 			else if (line.find("Port ", pos_beg) != std::string::npos)
// 			{
// 				pos_beg = line.find("Port") + 5;
// 				pos_end = line.find(";", pos_beg);
// 				conf.setPort(line.substr(pos_beg, pos_end - pos_beg));
// 			}
// 			else if (line.find("server_name ", pos_beg) != std::string::npos)
// 			{
// 				pos_beg = line.find("server_name ") + 12;
// 				pos_end = line.find(";", pos_beg);
// 				conf.setServerName(line.substr(pos_beg, pos_end - pos_beg));
// 			}
// 			else if (line.find("default_error_pages ", pos_beg) != std::string::npos)
// 			{
// 				pos_beg = line.find("default_error_pages ") + 20;
// 				pos_end = line.find(";", pos_beg);
// 				conf.setDefaultErrorPages(line.substr(pos_beg, pos_end - pos_beg));
// 			}
// 			else if (line.find("client_body_size ", pos_beg) != std::string::npos)
// 			{
// 				pos_beg = line.find("client_body_size ") + 17;
// 				pos_end = line.find(";", pos_beg);
// 				conf.setClientBodySize(line.substr(pos_beg, pos_end - pos_beg));
// 			}
// 			else
// 				break;
				
// 		}
// 	}
// 	file.close();
// }


int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "\033[31mMissing configuration file!\033[0m" << std::endl;
		return (1);
	}
	std::vector<Configuration> configs;
	std::vector<Socket> sockets;
	Configuration temp;
	int server_count;
	try
	{
		server_count = read_conf(argv[1], configs);
	}
	catch(std::string error)
	{
		std::cerr << error << '\n';
		return 1;
	}
	for (std::vector<Configuration>::iterator it = configs.begin(); it != configs.end(); ++it)
	{
		temp = *it;
		sockets.push_back(temp.getPort());
	}
	std::cout << server_count << std::endl;

	Socket socket(9999);
	int connection = socket.accept_socket();

	// Read from the connection
	char buffer[150000];
	read(connection, buffer, 150000);
	std::cout << "The message was: " << buffer;

	// Send a message to the connection
	std::string response = "Good talking to you\n";
	send(connection, response.c_str(), response.size(), 0);

	// Close the connections
	close(connection);
	// close(sockfd);


	return 0;
}