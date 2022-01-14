#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "webserv.hpp"
#include <map>

class Request
{
	private:
		int			code;
		bool		cgi;
		std::string port;
		std::string	method;
		std::string uri;
		std::string http_version;
		std::string	body;
		std::string	query_string;
		std::string script_path;
		std::map<std::string, std::string> headers;
		bool		post_file;
		bool		chunked;

	public:
		Request();
		Request(const Request& copy);
		Request& operator=(const Request& other);
		~Request();
		int getCode() const;
		void parseRequest(std::string line);
		void parse_first_line(std::string line);
		void add_headers(std::string line);
		void setHTTPversion(std::string line);
		void setBody(std::string line);
		void setCode(int code);
		bool getCGI() const;
		bool getPostFile() const;
		std::string	getMethod() const;
		std::string	getUri() const;
		std::string getHTTP_version() const;
		std::string getBody() const;
		std::string getPort() const;
		std::string	getQueryString() const;
		std::string getScriptPath() const;
		std::string setMethod(std::string line);
		std::string setURI(std::string line);
		std::map<std::string, std::string> getHeaders() const;
		std::string check_content_type();
		std::string	boundary;
};

std::ostream& operator<<(std::ostream& out, const Request& request);

#endif