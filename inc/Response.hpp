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

class Response
{
private:
	int		fd;
public:
	Response(int fd);
	~Response();

	void make_response(Request &request, std::vector<Configuration> config, int fd);
	void check_method(std::vector<Configuration> configs, Request &request);
};



#endif