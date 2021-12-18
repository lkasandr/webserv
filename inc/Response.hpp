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
#include <map>
#include <vector>

class Request;
class Configuration;

class Response
{
private:
	int					fd;
	int					status_code;
	std::string			code_description;
	std::string 		content_path;
	std::string			version;
	std::string			server;
	std::string			date;
	std::string			contentType;
	std::string			allow_method;
	std::string			location;
	std::string			connection;
	std::string			setCookie;
public:
	Response(int fd);
	// Response(const Response & copy);
	// Response& operator=(const Response & other);
	~Response();

	void make_response(Request *request, Configuration *config);
	void check_method(Configuration *configs, Request *request);
	void get_method(int fd, Configuration & conf);
	int	 find_config(std::vector<Configuration> configs, std::string URI);
	void check_errors(int code);

	//getters
	int getStatus_code() const;
	std::string getCodeDescription() const;
	std::string getVersion() const;
	std::string getServer() const;
	std::string getDate() const;
	std::string getAllow_method() const;

	std::string getContentPath(Configuration conf, std::string uri);
};

std::ostream& operator<<(std::ostream& out, const Response& response);


#endif