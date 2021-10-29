#include "Configuration.hpp"

Configuration::Configuration()
{
	this->host = "0.0.0.0";
	this->port = 8080;
	this->server_name = "localhost";
	this->default_error_pages = "./html/error.html";
	this->client_body_size = 30000;
}

// Configuration::Configuration(char *conf_path)
// {
// 	// read_conf(conf_path);
// }

Configuration::Configuration(const Configuration & copy)
{
	*this = copy;
}

Configuration& Configuration::operator=(const Configuration& other)
{
	if (this == &other)
		return (*this);
	this->host = other.getHost();
	this->port = other.getPort();
	this->server_name = other.getServerName();
	this->default_error_pages = other.getDefaultErrorPages();
	this->client_body_size = other.getClientBodySize();
	return *this;
}

Configuration::~Configuration()
{
}

std::string Configuration::getHost() const
{
	return this->host;
}
	
u_int	Configuration::getPort() const
{
	return this->port;
}

std::string Configuration::getServerName() const
{
	return this->server_name;
}

std::string Configuration::getDefaultErrorPages() const
{
	return this->default_error_pages;
}

int 		Configuration::getClientBodySize() const
{
	return this->client_body_size;
}

void	Configuration::setHost(std::string value) 
{
	this->host = value;
}

void	Configuration::setPort(std::string value) 
{
	this->port = atoi(value.c_str());
}

void		Configuration::setServerName(std::string value) 
{
	this->server_name = value;
}

void		Configuration::setDefaultErrorPages(std::string value) 
{
	this->default_error_pages = value;
}

void		Configuration::setClientBodySize(std::string value) 
{
	this->client_body_size = atoi(value.c_str());
}

// void		Configuration::setServCount(int count)
// {
// 	this->serv_count += count;
// }

// int			Configuration::getServCount() const
// {
// 	return this->serv_count;
// }
// void Configuration::read_conf(std::string const conf_path)
// {
// 	size_t pos_beg;
// 	size_t pos_end;
// 	std::string	line;
// 	std::ifstream	file(conf_path);
// 	// file.open(conf_path.c_str());
// 	// file.open(conf_path);
// 	if (!file.is_open())
// 		{
// 			std::cout << "\033[31mThe configuration file didn't open\033[0m" << std::endl;
// 			// return (1);
// 		}
// 	else if (file.peek() == EOF)
// 	{
// 		std::cout << "\033[31mThe configuration file is empty\033[0m" << std::endl;
	
// 	}
	
// 	while (!file.eof())
// 	{
// 		std::getline(file, line);
// 		pos_beg = 0;
// 		pos_end = 0;
// 		while (1)
// 		{
// 			pos_beg = line.find("Host:", pos_beg);
// 			if(pos_beg == std::string::npos)
// 				break;
// 			else
// 			{
// 				pos_beg = line.find("Host:") + 5;
//                 pos_end = line.find(";", pos_beg);
//                 this->host = line.substr(pos_beg, pos_end - pos_beg);
// 				pos_beg += host.length();
// 			}
			
				
// 		}
// 	}
// }

std::ostream& operator<<(std::ostream& out, const Configuration& config)
{
	out << "\nConfiguration:\n";
	out << "Host: " << config.getHost() << std::endl;
	out << "Port: " << config.getPort() << std::endl;
	out << "server_name: " << config.getServerName() << std::endl;
	out << "default_error_pages: " << config.getDefaultErrorPages() << std::endl;
	out << "client body size: " << config.getClientBodySize() << std::endl;
	// out << "serv_count" << config.getServCount() << std::endl;
	return (out);
}