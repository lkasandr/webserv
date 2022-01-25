#include "Configuration.hpp"

Configuration::Configuration()
{
	this->host = "0.0.0.0";
	this->port = 8080;
	this->server_name = "localhost";
	this->default_error_pages = "./html/error.html";
	this->client_body_size = 30000;
	this->http_method = "GET POST DELETE PUT";
	this->method_get = 0;
	this->method_post = 0;
	this->method_delete = 0;
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
	this->autoindex = other.getAutoindex();
	this->array = other.getArray();
	return *this;
}

Configuration::~Configuration()
{
}

////////////
// Getters//
////////////

const std::string &Configuration::getHost() const
{
	return this->host;
}
	
u_int	Configuration::getPort() const
{
	return this->port;
}

const std::string &Configuration::getServerName() const
{
	return this->server_name;
}

const std::string &Configuration::getDefaultErrorPages() const
{
	return this->default_error_pages;
}

int 		Configuration::getClientBodySize() const
{
	return this->client_body_size;
}

const std::string &Configuration::getHttpMethod() const
{
	return this->http_method;
}

int Configuration::getAutoindex() const
{
	return this->autoindex;
}

bool	Configuration::getGet() const
{
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

const std::string	&Configuration::getCGI() const
{
	return this->CGI_pass;
}

const std::vector<location>	&Configuration::getArray() const
{
	return this->array;
}

//////////////
// Setters ///
//////////////

void	Configuration::setHost(const std::string &value) 
{
	this->host = value;
}

void	Configuration::setPort(const std::string &value) 
{
	this->port = atoi(value.c_str());
}

void		Configuration::setServerName(const std::string &value) 
{
	this->server_name = value;
}

void		Configuration::setDefaultErrorPages(const std::string &value) 
{
	this->default_error_pages = value;
}

void		Configuration::setClientBodySize(const std::string &value) 
{
	this->client_body_size = atoi(value.c_str());
}

void	Configuration::setHttpMethod(const std::string &value)
{
	this->http_method = value;
}

void		Configuration::setCGI(const std::string &value)
{
	this->CGI_pass = value;
}

void Configuration::setAutoindexOff(void)
{
	this->autoindex = 0;
}

void Configuration::setAutoindexOn(void)
{
	this->autoindex = 1;
}

void Configuration::setArray(const std::string &_location,const std::string &root,const std::string &index,const std::string &client_body_size,const std::string &http_method)
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
	return (out);
}