#include "Response.hpp"


Response::Response(int fd): fd(fd) {}

// Configuration	find_config(std::vector<Configuration> configs, std::string URI)
// {
// 	for (std::vector<Configuration>::iterator it = configs.begin(); it != configs.end(); ++it)
// 	{
// 		if (URI.find(it->getlocation()) != std::string::npos)
// 			return (*it);
// 	}
// }



void Response::make_response(Request *request, std::vector<Configuration> config)
{
	std::string URI = request->getUri();
	if (URI == "/")
		URI = "/home";
	// this->config = find_config(config, URI);
	// check errors
	
	this->status_code = request->getCode();
	check_method(config, request);
}


void Response::get_method(int fd, Configuration & conf)
{
	std::ifstream	file;
	file.open(conf.getIndex());
	if (!file.is_open())
		std::cout << "error html" << std::endl;
	// std::stringstream content;
	this->content << file.rdbuf();
	std:: stringstream response;
	response << "HTTP/1.1 200 OK\r\n" << "Version: HTTP/1.1\r\n"
	<< "Content-Type: text/html; charset=utf-8\r\n"
	<< "Content-Length: " << this->content.str().length()
	<< "\r\n\r\n"
	<< this->content.str();
	send(fd, response.str().c_str(), response.str().length(), 0);
	file.close();
}


void Response::check_method(std::vector<Configuration> configs, Request *request)
{
	std::string URI = request->getUri();
	if (URI == "/ ")
		URI = "/home";
	std::string 	method[3] = {"GET", "POST", "DELETE"};
	int m = 0;
	while(m < 3 && request->getMethod().compare(method[m]) != 0 )
		m++;
	switch (m)
	{
	case 0:
		std::cout << "Method GET" << std::endl;
		for (std::vector<Configuration>::iterator it = configs.begin(); it != configs.end(); ++it)
		{
			if (URI.find("favicon"))
				std::cout << "favicon" << request->getUri() << std::endl;
			if (URI.find(it->getlocation()) != std::string::npos)
			{	
				if (it->checkGet())
					get_method(this->fd, *it);
				else
				{
					std::cout << "Method not allowed" << std::endl;
					// 405 error html
				}
			}
		}
		break;
	case 1:
		std::cout << "Method POST" << std::endl;
		// for (std::vector<Configuration>::iterator it = configs.begin(); it != configs.end(); ++it)
		// {
		// 	if (it->getPort() == request.getPort())
		// 	{	
		// 		if (it->checkPost())
		// 			post_method(fd);
		// 		else
		// 			std::cout << "Method not allowed" << std::endl; // 405 error html
		// 	}
		// }
		break;
	case 2:
		std::cout << "Method DELETE" << std::endl;
		// for (std::vector<Configuration>::iterator it = configs.begin(); it != configs.end(); ++it)
		// {
		// 	if (it->getPort() == request.getPort())
		// 	{	
		// 		if (it->checkDelete())
		// 			delete_method(fd);
		// 		else
		// 			std::cout << "Method not allowed" << std::endl; // 405 error html
		// 	}
		// }
		break;
	default:
		std::cout << "Uncknown method" << std::endl;	//501 not implemented
		break;
	}	
}

Response::~Response()
{
}
