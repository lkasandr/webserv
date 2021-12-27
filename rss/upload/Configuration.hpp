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
	std::string		http_method;
	bool			method_get;
	bool			method_post;
	bool			method_delete;
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
	std::string	getHttpMethod() const;
	bool		getGet() const;
	bool		getPost() const;
	bool		getDelete() const;
	// int			getServCount() const;

	void		setHost(std::string value) ;
	void		setPort(std::string value);
	void		setServerName(std::string value) ;
	void		setDefaultErrorPages(std::string value) ;
	void		setClientBodySize(std::string value) ;
	void		setHttpMethod(std::string value);
	// void		setServCount(int count);

	bool		checkGet();
	bool		checkPost();
	bool		checkDelete();
};

std::ostream& operator<<(std::ostream& out, const Configuration& config);

#endif
