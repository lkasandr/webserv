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
	std::string http_version;
	std::string protocol_headers;
	std::string	protocol_body;
	int			code;

public:
	Request();
	Request(const Request& copy);
	Request& operator=(const Request& other);
	~Request();

	void		parseRequest(char *buffer);

	std::string	getMethod() const;
	std::string	getUri() const;
	std::string	getHost() const;
	std::string getHTTP_version() const;
	std::string	getPort() const;
	std::string getHeaders() const;
	std::string getBody() const;
	int			getCode() const;

	void		setMethod(std::string line);
	void		setUri(std::string line);
	void		setHTTP_version(std::string line);
	void		setHost(std::string line);
	void		setPort(std::string line);
	
};





#endif