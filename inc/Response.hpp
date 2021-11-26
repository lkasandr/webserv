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



class Request;


class Response
{
private:
	int					fd;
	int					status_code;
	std::stringstream 	content;
	std::string			server;
	std::string			date;
	std::string			contentType;
	std::string			contentLength;
	std::string			lastModified;
	std::string			connection;
	std::string			transferEncoding;
	std::string			setCookie;
	
public:
	Response(int fd);
	// Response(const Response & copy);
	// Response& operator=(const Response & other);
	~Response();

	void make_response(Request *request, std::vector<Configuration> config);
	void check_method(std::vector<Configuration> configs, Request *request);
	void get_method(int fd, Configuration & conf);
};



#endif