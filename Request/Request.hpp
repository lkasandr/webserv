#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <string>
#include <iostream>
#include <istream>
#include <map>

class Request
{
private:
	int			code;
	std::string	method;
	std::string uri;
	std::string http_version;



	std::string	host;
	
	std::string protocol_headers;
	std::string	protocol_body;
	std::map<std::string, std::string> headers;

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
	// std::string getHeaders() const;
	std::string getBody() const;
	int getCode() const;
	std::map<std::string, std::string> getHeaders() const;


	void parse_first_line(std::string line);
	std::string setMethod(std::string line);
	std::string setURI(std::string line);
	void setHTTPversion(std::string line);

	void		setHost(std::string line);
	void		setPort(std::string line);
	
};





#endif