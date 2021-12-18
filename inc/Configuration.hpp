#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP


#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <map>

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
	// std::string		location;
	std::string location;
	std::string root;
	std::map<std::string, std::string> path;
	std::string		index;
	std::string		CGI_pass;
public:
	Configuration();
	Configuration(char *conf_path);
	Configuration(const Configuration & copy );
	Configuration& operator=(const Configuration& other);
	~Configuration();

	std::string getHost() const;
	u_int		getPort() const;
	std::string getServerName() const;
	std::string getDefaultErrorPages() const;
	int 		getClientBodySize() const;
	std::string	getHttpMethod() const;
	bool		getGet() const;
	bool		getPost() const;
	bool		getDelete() const;
	std::string getLocation() const;
	std::string getRoot() const;
	std::map<std::string, std::string> getPath() const;
	// std::string getLocation() const;
	std::string	getIndex() const;
	std::string	getCGI() const;

	void		setHost(std::string value) ;
	void		setPort(std::string value);
	void		setServerName(std::string value) ;
	void		setDefaultErrorPages(std::string value) ;
	void		setClientBodySize(std::string value) ;
	void		setHttpMethod(std::string value);
	void		setLocation(std::string value);
	void		setIndex(std::string value);
	void		setCGI(std::string value);
	void		setRoot(std::string value);
	void		setPath(std::string location, std::string root);
	bool		checkGet();
	bool		checkPost();
	bool		checkDelete();
};

std::ostream& operator<<(std::ostream& out, const Configuration& config);

#endif