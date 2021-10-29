#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP


#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

class Configuration
{
private:
	std::string		host;
	u_int			port;
	std::string		server_name;
	std::string		default_error_pages;
	int				client_body_size;
	// int				serv_count;		
public:
	Configuration();
	Configuration(char *conf_path);
	Configuration(const Configuration & copy );
	Configuration& operator=(const Configuration& other);
	~Configuration();

	// void		read_conf(std::string const conf_path);
	std::string getHost() const;
	u_int		getPort() const;
	std::string getServerName() const;
	std::string getDefaultErrorPages() const;
	int 		getClientBodySize() const;
	// int			getServCount() const;

	void		setHost(std::string value) ;
	void		setPort(std::string value);
	void		setServerName(std::string value) ;
	void		setDefaultErrorPages(std::string value) ;
	void		setClientBodySize(std::string value) ;
	// void		setServCount(int count);
};

std::ostream& operator<<(std::ostream& out, const Configuration& config);

#endif