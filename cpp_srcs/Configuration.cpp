#include "Configuration.hpp"

Configuration::Configuration()
{
	this->host = "0.0.0.0";
	this->port = 8080;
	this->server_name = "localhost";
	this->default_error_pages = "./html/error.html";
	this->client_body_size = 30000;
	this->http_method = "GET POST DELETE";
	this->method_get = 0;
	this->method_post = 0;
	this->method_delete = 0;
	// this->location = "home";
	// this->index = "./rss/home/index.html";
	// this->path.clear();
	this->autoindex = 0;
}

Configuration::Configuration(const Configuration & copy)
{
	*this = copy;
}

Configuration& Configuration::operator=(const Configuration& other)
{
	if (this == &other)
		return (*this);
	this->host = other.getHost();
	this->port = other.getPort();
	this->server_name = other.getServerName();
	this->default_error_pages = other.getDefaultErrorPages();
	this->client_body_size = other.getClientBodySize();
	this->http_method = other.getHttpMethod();
	this->method_get = other.getGet();
	this->method_post = other.getPost();
	this->method_delete = other.getDelete();
	// this->location = other.getLocation();
	// this->index = other.getIndex();
	this->autoindex = other.getAutoindex();
	// this->path = other.getPath();
	this->array = other.getArray();
	return *this;
}

Configuration::~Configuration()
{
}

////////////
// Getters//
////////////

std::string Configuration::getHost() const
{
	return this->host;
}
	
u_int	Configuration::getPort() const
{
	return this->port;
}

std::string Configuration::getServerName() const
{
	return this->server_name;
}

std::string Configuration::getDefaultErrorPages() const
{
	return this->default_error_pages;
}

int 		Configuration::getClientBodySize() const
{
	return this->client_body_size;
}

std::string Configuration::getHttpMethod() const
{
	return this->http_method;
}

int Configuration::getAutoindex() const
{
	return this->autoindex;
}

bool	Configuration::getGet() const
{
	// this->checkGet(this->http_method);
	return this->method_get;
}


bool	Configuration::getPost() const
{
	return this->method_post;
}


bool	Configuration::getDelete() const
{
	return this->method_delete;
}

bool	Configuration::getPut() const
{
	return this->method_put;
}

// std::string Configuration::getLocation() const
// {
// 	return this->location;
// }

// std::string Configuration::getRoot() const
// {
// 	return this->root;
// }

// std::string Configuration::getIndex() const
// {
// 	return this->index;
// }

std::string	Configuration::getCGI() const
{
	return this->CGI_pass;
}

// std::map<std::string, std::string> Configuration::getPath() const
// {
	// вывод map Path:
	// std::map<std::string, std::string>::const_iterator it;
	// std::cout << "Location + root: " << std::endl;
	// for (it=this->path.begin(); it!=this->path.end(); it++)
	// 	std::cout << "\033[35m" << it->first << ' ' << it->second << "\033[0m" << std::endl;
// 	return this->path;
// }

std::vector<location>	Configuration::getArray() const
{
	return this->array;
}

// struct location	Configuration::getLocationStruct() const
// {
// 	return this->location_struct;
// }

//////////////
// Setters ///
//////////////

void	Configuration::setHost(std::string value) 
{
	this->host = value;
}

void	Configuration::setPort(std::string value) 
{
	this->port = atoi(value.c_str());
}

void		Configuration::setServerName(std::string value) 
{
	this->server_name = value;
}

void		Configuration::setDefaultErrorPages(std::string value) 
{
	this->default_error_pages = value;
}

void		Configuration::setClientBodySize(std::string value) 
{
	this->client_body_size = atoi(value.c_str());
}

void	Configuration::setHttpMethod(std::string value)
{
	this->http_method = value;
}

// void		Configuration::setLocation(std::string value) 
// {
// 	this->location = value;
// 	// this->location.push_back(value);
// }

// void		Configuration::setIndex(std::string value) 
// {
// 	this->index = value;
// }

void		Configuration::setCGI(std::string value)
{
	this->CGI_pass = value;
}

// void		Configuration::setRoot(std::string value)
// {
// 	// this->root.push_back(value);
// 	this->root = value;
// }

void Configuration::setAutoindexOff(void)
{
	this->autoindex = 0;
}

void Configuration::setAutoindexOn(void)
{
	this->autoindex = 1;
}

// void	Configuration::setPath(std::string location, std::string root)
// {
// 	// std::cout << location << " " << root << std::endl;
// 	this->path.insert(std::pair<std::string, std::string>(location, root));
// 	// std::cout << this->path[location] << std::endl;

// }

void Configuration::setArray(std::string _location, std::string root, std::string index, std::string client_body_size, std::string http_method)
{
	location temp;

	temp.location = _location;
	temp.root = root;
	temp.index = index;
	if (client_body_size.empty())
		temp._client_body_size = getClientBodySize();
	else
		temp._client_body_size = atoi(client_body_size.c_str());
	if (http_method.empty())
		temp._http_method = getHttpMethod();
	else
		temp._http_method = http_method;
	// std::cout << temp.location << " clien body size:" << temp._client_body_size << " http method: " <<  temp._http_method << std::endl;
	this->array.push_back(temp);
}

/////////////////
//////Methods///
////////////////

bool Configuration::checkGet()
{
	if(this->http_method.find("GET") != std::string::npos)
		this->method_get = true;
	return this->method_get;
}

bool Configuration::checkPost()
{
	if(this->http_method.find("POST") != std::string::npos)
		this->method_post = true;
	return this->method_post;
}

bool Configuration::checkDelete()
{
	if(this->http_method.find("DELETE") != std::string::npos)
		this->method_delete = true;
	return this->method_delete;
}

bool Configuration::checkPut()
{
	if(this->http_method.find("PUT") != std::string::npos)
		this->method_put = true;
	return this->method_put;
}

std::ostream& operator<<(std::ostream& out, const Configuration& config)
{
	out << "\nConfiguration:\n";
	out << "Host: " << config.getHost() << std::endl;
	out << "Port: " << config.getPort() << std::endl;
	out << "server_name: " << config.getServerName() << std::endl;
	out << "default_error_pages: " << config.getDefaultErrorPages() << std::endl;
	out << "client body size: " << config.getClientBodySize() << std::endl;
	out << "HTTP methods: " << config.getHttpMethod() << std::endl;
	// out << "Location: " << config.getLocation() << std::endl;
	// вывод map Path:
	// std::map<std::string, std::string> path;
	// path = config.getPath();
	// std::map<std::string, std::string>::iterator it;
	// std::cout << "Location + root: " << std::endl;
	// for (it=path.begin(); it!=path.end(); it++)
	// 	std::cout << "\033[35m" << config.getPath()["/"] << "\033[0m" << std::endl;
	
	// out << "Index: " << config.getIndex() << std::endl;
	return (out);
}