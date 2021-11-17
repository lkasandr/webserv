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



std::ostream& operator<<(std::ostream& out, const Configuration& config)
{
	out << "\nConfiguration:\n";
	out << "Host: " << config.getHost() << std::endl;
	out << "Port: " << config.getPort() << std::endl;
	out << "server_name: " << config.getServerName() << std::endl;
	out << "default_error_pages: " << config.getDefaultErrorPages() << std::endl;
	out << "client body size: " << config.getClientBodySize() << std::endl;
	out << "HTTP methods: " << config.getHttpMethod() << std::endl;
	// out << "serv_count" << config.getServCount() << std::endl;
	return (out);
}