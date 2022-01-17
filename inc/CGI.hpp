#ifndef CGI_HPP
# define CGI_HPP

#include "webserv.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include <sys/wait.h>
#include <vector>
#include <string>
#include <iostream>
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <fcntl.h>
#include <fstream> 
#include <map>
#include <list>
#include <sstream>
#include <bitset>
#include <cstring> 
#include <cstdlib> 
#include <unistd.h>
#include <algorithm> 

# define CGI_BUF_SIZE 65536
# define PY 0
# define PHP 1
# define BLA 2
# define DEFAULT 3


class Response;
class Request;

class CgiProcess 
{
	private:
		std::map<std::string, std::string>	env_map;
		char								**env_array;
		const Request&						request;
		const Response&						response;
		std::string							body;
		int							status;

		CgiProcess();
		void		initEnv();
		void		fillEnv();
		std::map<std::string, std::string>	change_headers(std::map<std::string, std::string> &headers);

	public:
		CgiProcess(Request const& req, Response const& res);
		CgiProcess(CgiProcess const & copy);
		~CgiProcess();
		CgiProcess& operator=(CgiProcess const & other);
		int	execCGI(const std::string & cgi_path);
		const std::string &getBody(void);
		int getStatus();
		std::string	get_cwd();

};

std::ostream & operator<<(std::ostream & out, CgiProcess const & cgi_proc);

#endif 
