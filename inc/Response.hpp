#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <string>
#include <iostream>
#include <istream>
#include "Request.hpp"
#include "Server.hpp"
#include "Socket.hpp"
#include "Configuration.hpp"
#include "webserv.hpp"
#include "CGI.hpp"
#include <fstream>
#include <sstream>
#include <cstdio>
#include <map>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>

#include <sys/types.h>
#include <fcntl.h>

class Request;
class Configuration;

class Response
{
private:
	int					fd;
	int					status_code;
	size_t				contentLength;
	std::string			code_description;
	std::string 		content_path;
	std::string			version;
	std::string			server;
	std::string			date;
	std::string			contentType;
	std::string			allow_method;
	std::string			_location;
	std::string			connection;
	std::string			setCookie;
public:
	Response(int fd);
	~Response();

	void make_response(Request *request, Configuration *config);
	void check_method(Configuration *configs, Request *request);
	void get_method(int fd, Configuration & conf);
	int	 find_config(std::vector<Configuration> configs, std::string URI);
	void check_errors(int code);
	std::string makeAutoindexPage(const char *path, std::string const &host);

	//getters
	int getStatus_code() const;
	const std::string &getCodeDescription() const;
	const std::string &getVersion() const;
	const std::string &getServer() const;
	const std::string &getDate() const;
	const std::string &getAllow_method() const;
	const std::string &getContentType() const;
	size_t		getContentLength() const;
	std::string getContentPath(Configuration &conf, std::string &uri) ;
};

std::ostream& operator<<(std::ostream& out, const Response& response);


#endif