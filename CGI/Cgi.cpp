#include "CGI.hpp"

CgiProcess::CgiProcess(Request const& req, Response const& res) :
env_array(NULL), request(req), response(res)
{
	this->initEnv();
}

CgiProcess::CgiProcess(CgiProcess const & copy) :
env_array(copy.env_array), request(copy.request), response(copy.response),
body(copy.body), status(copy.status)
{}

CgiProcess::~CgiProcess()
{
	for (size_t i = 0; i < this->env_map.size(); ++i)
		delete[] this->env_array[i];
	delete[] this->env_array;
}

CgiProcess& CgiProcess::operator=(CgiProcess const & other)
{
	this->env_array = other.env_array;
	// this->request = other.request;
	// this->response = other.response;
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
	this->env_map["REDIRECT_STATUS"]	=	"200";
	this->env_map["CONTENT_LENGTH"]		=	to_string(this->request.getBody().size());
	this->env_map["CONTENT_TYPE"]		=	headers["content-type"];
	this->env_map["GATEWAY_INTERFACE"]	=	"CGI/1.1";
	this->env_map["PATH_INFO"]			=	this->request.getScriptPath();	// rfc3875  4.1.5.
	this->env_map["PATH_TRANSLATED"]	=	this->request.getScriptPath(); //
	this->env_map["QUERY_STRING"]		=	this->request.getQueryString();	// после "?" В URL
	this->env_map["REMOTE_ADDR"]		=	/*"127.0.0.1";	*/this->request.getHeaders().find("Host")->second;
	this->env_map["REQUEST_METHOD"]		=	this->request.getMethod();
	this->env_map["SCRIPT_NAME"]		=	this->request.getScriptPath();	//  variable MUST be set to a URI path
	this->env_map["SCRIPT_FILENAME"]	=	this->request.getScriptPath();	//  variable MUST be set to a URI path
	this->env_map["SERVER_NAME"]		=	this->request.getHeaders().find("Host")->second;	// hostname
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

int	CgiProcess::execCGI(std::string const& cgi_path)
{
	int fd[2][2];
    char buf[100000];
    int len = -1;

	std::string path;  
  	std::string script;

	this->fillEnv();
	int i = 0;
	while (this->env_array[i])
	{
		std::cout << "ENV: " << this->env_array[i] << std::endl;
		i++;
	}
    bzero(buf, 100000);
    pipe(fd[0]);
    pipe(fd[1]);
    write(fd[0][1], (char *)this->body.c_str(), this->body.length());
    close(fd[0][1]);
    if(fork() == 0)
    {
        dup2(fd[0][0], 0);
        close(fd[0][0]);
        dup2(fd[1][1], 1);
        close(fd[1][1]);
		
		if (cgi_path.find(".py") != std::string::npos)
		{
			path = "/usr/bin/python3";  
  			script = "./cgi/print_env.py";  
 			
 			char * a[3] = {
			const_cast<char*>(script.c_str()),
			const_cast<char*>(script.c_str()),
			(char *)0 };

			execve(path.c_str(), &a[0], this->env_array);
			exit(0);
		}

		if (cgi_path.find("php") != std::string::npos)
		{
			path = "/usr/bin/php-cgi7.4";  
  			script = "./cgi/phpinfo.php"; 

 			// char * a[3] = {
			// const_cast<char*>(path.c_str()),
			// const_cast<char*>(script.c_str()),
			// (char *)0 };
			// execve(path.c_str(), &a[0], this->env_array);
			// exit(0);
		}
		else
			path = cgi_path;

		std::cout << "PATH " << path << std::endl;
		std::cout << "SCRIPT " << script << std::endl;
		char * argv[3] = {
		const_cast<char*>(script.c_str()),
		const_cast<char*>(script.c_str()),
		(char *)0 };
		execve(path.c_str(), &argv[0], this->env_array);
		exit(0);
    }
    close(fd[1][1]);
	// this->body.clear();
    while((len = read(fd[1][0], buf, 100000)) != 0)
    {
        if (len == -1)
            std::cout << "“Read error CGI”" << std::endl;
        // this->body.clear();
        for(int i = 0;i < len; i++)
            this->body.push_back(buf[i]);
        bzero(buf, 100000);
    }
	wait(0);
    close(fd[0][0]);
    close(fd[1][0]);
	// std::cout << "259 BODY " << this->body << std::endl;
	return 0;
}



std::string CgiProcess::getBody(void)
{
	return this->body;
}

std::string				CgiProcess::getStatus(void)
{
	return this->status;
}
