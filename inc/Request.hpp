#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <string>
#include <iostream>
#include <istream>

class Request
{
private:
	std::string	first_line;
	std::string	method;
	std::string uri;
	std::string	host;
	std::string protocol_headers;
	std::string	protocol_body;

public:
	Request();
	Request(const Request& copy);
	Request& operator=(const Request& other);
	~Request();

	std::string	getMethod() const;
	std::string	getUri() const;
	std::string	getHost() const;
	std::string getHeaders() const;
	std::string getBody() const;

	void		setMethod(std::string line);
	void		setUri(std::string line);

	void		parseRequest(char *buffer);
};





#endif