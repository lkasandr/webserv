#include "CGI.hpp"

CgiProcess::CgiProcess(Request const& req, Response const& res):env_array(NULL), request(req), response(res), status(200), cookies(false)
{
	this->initEnv();
}

CgiProcess::CgiProcess(CgiProcess const & copy):env_array(copy.env_array), request(copy.request), response(copy.response),\
body(copy.body), status(copy.status), cookies(false) {}

CgiProcess::~CgiProcess()
{
	for (size_t i = 0; i < this->env_map.size(); ++i)
		delete[] this->env_array[i];
	delete[] this->env_array;
}

CgiProcess& CgiProcess::operator=(CgiProcess const & other)
{
	this->env_array = other.env_array;
	this->body = other.body;
	this->status = other.status;

	return *this;
}

std::map<std::string, std::string> CgiProcess::change_headers(std::map<std::string, std::string> &headers)
{
	std::map<std::string, std::string> http_headers;
	std::map<std::string, std::string>::const_iterator it = headers.begin();
	std::map<std::string, std::string>::const_iterator end = headers.end();
	std::string key;
	while (it != end)
	{
		key = "HTTP_";
		transform(it->first.begin(), it->first.end(), std::back_inserter(key), toupper);
		std::replace(key.begin(), key.end(), '-', '_');
		http_headers[key] = it->second;
		++it;
	}
	return (http_headers);
}

std::string	CgiProcess::get_cwd(void)
{
	char cwd[256];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
      return (std::string("./"));
    else
      return (std::string(cwd));  
}

std::string to_string(size_t size)
{
    std::stringstream ss;
    ss << size;
    return (ss.str());
}

void	CgiProcess::initEnv(void)
{
	std::map<std::string, std::string> headers = this->request.getHeaders();
	std::map<std::string, std::string> http_headers = this->change_headers(headers);
	this->env_map["AUTH_TYPE"]			=	"";
	this->env_map["REDIRECT_STATUS"]	=	"200";
	this->env_map["CONTENT_LENGTH"]		=	headers["Content-Length"];	
	this->env_map["CONTENT_TYPE"]		=	headers["Content-Type"];
	this->env_map["GATEWAY_INTERFACE"]	=	"CGI/1.1";
	this->env_map["PATH_INFO"]			=	this->request.getUri();	// rfc3875  4.1.5.
	this->env_map["PATH_TRANSLATED"]	=	"/rss/cgi";		//
	this->env_map["QUERY_STRING"]		=	this->request.getQueryString();	// после "?" В URL
	this->env_map["REMOTE_ADDR"]		=	"127.0.0.1";	//this->request.getHeaders().find("Host")->second;
	this->env_map["REMOTE_HOST"]		=	"localhost";
	this->env_map["REMOTE_IDENT"];
	this->env_map["REMOTE_USER"];
	this->env_map["REQUEST_METHOD"]		=	this->request.getMethod();
	this->env_map["REQUEST_URI"]		=	this->request.getUri();
	this->env_map["SCRIPT_NAME"]		=	this->request.getScriptPath();	//  variable MUST be set to a URI path
	this->env_map["SCRIPT_FILENAME"]	=	this->request.getScriptPath();	//  variable MUST be set to a URI path
	this->env_map["SERVER_NAME"]		=	headers["Host"];
	this->env_map["SERVER_PORT"]		=	this->request.getPort();	// variable MUST be set
	this->env_map["SERVER_PROTOCOL"]	=	this->request.getHTTP_version();
	this->env_map["SERVER_SOFTWARE"]	=	"webserv";
	this->env_map["UPLOAD_DIR"]			=	"/rss/upload";
	this->env_map["DOCUMENT_ROOT"]		=	this->get_cwd(); 
	this->env_map.insert(http_headers.begin(), http_headers.end());
}

void	CgiProcess::fillEnv(void)
{
	this->env_array = new char*[this->env_map.size() + 1];
	std::map<std::string, std::string>::const_iterator it = this->env_map.begin();
	std::string str;
	size_t i = 0;
	while(i < this->env_map.size())
	{
		str = it->first + '=' + it->second;
		this->env_array[i] = new char[str.size() + 1];
		strcpy(this->env_array[i], str.c_str());
		str.clear();
		++it;
		++i;
	}
	this->env_array[i] = NULL;
}

int check_extension(std::string const& cgi_path)
{
	if(cgi_path.find(".py") != std::string::npos)
		return PY;
	else if (cgi_path.find(".php") != std::string::npos)
		return PHP;
	else if (cgi_path.find(".bla") != std::string::npos)
		return BLA;
	else
		return DEFAULT;
}

int	CgiProcess::execCGI(const std::string & cgi_path)
{
    char buf[100000];
    int len = -1;
	std::string path;  
  	std::string script;
	std::string root_directory;

	this->fillEnv();
    bzero(buf, 100000);
    
	FILE *file_In = tmpfile();
    FILE *file_Out = tmpfile();
    long fdIn = fileno(file_In);
    long fdOut = fileno(file_Out);
	
	write(fdIn, const_cast<char*>(this->request.getBody().c_str()), this->request.getBody().length());
	lseek(fdIn, 0, SEEK_SET);
    if(fork() == 0)
    {
		dup2(fdIn, STDIN_FILENO);
        dup2(fdOut, STDOUT_FILENO);

		int m = check_extension(cgi_path);
		switch (m)
		{
		case PY:
			path = "/usr/bin/python3";  
			script = this->request.getScriptPath();; 
			root_directory = get_cwd() + request.getUri().substr(0, request.getUri().find_last_of("/"));
			chdir(root_directory.c_str());
			break;
		case PHP:
			path = "/usr/bin/php-cgi";  
  			script = this->request.getScriptPath();; 
			root_directory = get_cwd() + request.getUri().substr(0, request.getUri().find_last_of("/"));
			chdir(root_directory.c_str());
			break;
		case BLA:
			if (OStype == 2)
			{
				path = "./rss/directory/ubuntu_cgi_tester";  
  				script = "./rss/directory/ubuntu_cgi_tester";
			}
			else
			{
				path = "./rss/directory/cgi_tester";  
  				script = "./rss/directory/cgi_tester";
			}
			break;
		default:
			path = cgi_path;
			script = cgi_path;
			break;
		}
		char * argv[3] = {
		const_cast<char*>(script.c_str()),
		const_cast<char*>(script.c_str()),
		(char *)0 };
		if (execve(path.c_str(), &argv[0], this->env_array) < 0)
		{
			this->status = 502;
			exit(1);
		}
		exit(0);
    }
    wait(0);
	this->body.clear();
	lseek(fdOut, 0, SEEK_SET);
    while((len = read(fdOut, buf, 10000)) != 0)
    {
		if (len == -1)
        {
			std::cout << "CGI ERROR" << std::endl;
			this->status = 502;
		}
		this->body.append(buf, len);
        bzero(buf, 10000);
    }
	fclose(file_In);
    fclose(file_Out);
    close(fdIn);
    close(fdOut);
	return 0;
}

const std::string &CgiProcess::getBody(void)
{
	size_t pos= this->body.find("\r\n\r\n") + 4;
	if (pos != std::string::npos)
	{
		if(this->body.substr(0, pos).find("Set-Cookie") != std::string::npos)
		{
			this->cookies = true;
			return this->body;
		}
		else
			this->body = this->body.substr(pos); //remove headers
	}
	return this->body;
}

int	CgiProcess::getStatus(void)
{
	return this->status;
}

bool	CgiProcess::getCookies(void)
{
	return this->cookies;
}