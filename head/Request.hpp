#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "../inc/webserv.hpp"

class Request
{
	private:
		int			code;
		bool		cgi;
		std::string	method;
		std::string uri;
		std::string http_version;
		std::string	body;
		std::map<std::string, std::string> headers;
		bool		post_file;

	public:
		Request();
		Request(const Request& copy);
		Request& operator=(const Request& other);
		~Request();
		int getCode() const;
		void parseRequest(char *buffer);
		void parse_first_line(std::string line);
		void add_headers(std::string line);
		void setHTTPversion(std::string line);
		void setBody(std::string line);
		bool getCGI() const;
		bool getPostFile() const;
		std::string	getMethod() const;
		std::string	getUri() const;
		std::string getHTTP_version() const;
		std::string getBody() const;
		std::string setMethod(std::string line);
		std::string setURI(std::string line);
		std::map<std::string, std::string> getHeaders() const;
		std::string check_content_type();
		std::string	boundary;
};

#endif