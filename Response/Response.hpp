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
#include <fstream>
#include <sstream>
#include <cstdio>



class Request;
class Configuration;


class Response
{
private:
	int					fd;
	int					status_code;
	std::string			code_description;
	// Configuration		config;
	std::string 		content_path;
	// std::stringstream  response;
	std::string			version;
	std::string			server;
	std::string			date;
	std::string			contentType;
	std::string			allow_method;
	// std::string			contentLength;
	// std::string			lastModified;
	std::string			connection;
	// std::string			transferEncoding;
	std::string			setCookie;
	// std::vector<char> 	favicon_buffer;
public:
	Response(int fd);
	// Response(const Response & copy);
	// Response& operator=(const Response & other);
	~Response();

	void make_response(Request *request, std::vector<Configuration> config);
	void check_method(std::vector<Configuration> configs, Request *request);
	void get_method(int fd, Configuration & conf);
	int	 find_config(std::vector<Configuration> configs, std::string URI);
	void check_errors(int code);
};

std::ostream& operator<<(std::ostream& out, const Response& response);


#endif