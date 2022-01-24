#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP


#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <map>

#include <vector>

typedef struct {
	std::string location;
	std::string root;
	std::string index;
	int _client_body_size;
	std::string _http_method;
} location;

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
	bool			method_put;
	std::string		index;
	std::string		CGI_pass;
	int				autoindex;

	location		location_struct;

	std::vector<location> array;

public:
	Configuration();
	Configuration(char *conf_path);
	Configuration(const Configuration & copy );
	Configuration& operator=(const Configuration& other);
	~Configuration();

	const std::string &getHost() const;
	u_int		getPort() const;
	const std::string &getServerName() const;
	const std::string &getDefaultErrorPages() const;
	int 		getClientBodySize() const;
	const std::string &getHttpMethod() const;
	bool		getGet() const;
	bool		getPost() const;
	bool		getDelete() const;
	bool		getPut() const;
	int			getAutoindex() const;
	const std::string &getCGI() const;

	const std::vector<location>	&getArray() const;

	void		setHost(const std::string &value) ;
	void		setPort(const std::string &value);
	void		setServerName(const std::string &value) ;
	void		setDefaultErrorPages(const std::string &value) ;
	void		setClientBodySize(const std::string &value) ;
	void		setHttpMethod(const std::string &value);
	void		setLocation(const std::string &value);
	void		setCGI(const std::string &value);
	void		setAutoindexOn(void);
	void		setAutoindexOff(void);

	void 		setArray(const std::string &location,const std::string &root,const std::string &index,const std::string &client_body_size,const std::string &http_method);

	bool		checkGet();
	bool		checkPost();
	bool		checkDelete();
	bool		checkPut();
};

std::ostream& operator<<(std::ostream& out, const Configuration& config);

#endif