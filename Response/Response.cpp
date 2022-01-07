#include "Response.hpp"
#include <string.h>
#include <errno.h>
#include <string>
#include <cstdio>

Response::Response(int fd): fd(fd) 
{
	this->status_code = 200;
	this->code_description = " OK\r\n";
	this->version = "HTTP/1.1 ";
	this->server = "Server: webserv\r\n";
	this->connection = "Connection: Keep-Alive\r\n";
	this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
	this->allow_method = "";
	this->setCookie = "";
}

std::string get_date()
{
  char buff[64] = { 0 };
  time_t current_time;
  tm    *current_tm;
  time(&current_time);
  current_tm = localtime(&current_time);
  strftime(buff, sizeof buff, "%a, %d %b %Y %H:%M:%S GMT", current_tm);
  return std::string(buff);
}

void Response::check_errors(int code)
{
	switch (code)
	{
	case 204:	// No Content
		this->code_description = " No Content\r\n";
		this->content_path = "./rss/error/204.html";
		this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
		break;
	case 301: // Moved Permanently
		this->code_description = " Moved Permanently\r\n";
		this->content_path = "./rss/error/301.html";
		this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
		break;
	case 400: // Bad Request 
		this->code_description = " Bad Request\r\n";
		this->content_path = "./rss/error/400.html";
		this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
		break;
	case 404: // Not Found
		this->code_description = " Not Found\r\n";
		this->content_path = "./rss/error/404.html";
		this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
		this->connection = "Connection: Close\r\n";
		break;
	case 405: // Method Not Allowed 
		this->code_description = " Method Not Allowed\r\n";
		this->content_path = "./rss/error/405.html";
		this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
		break;
	case 413: // Payload Too Large
		this->code_description = " Payload Too Large\r\n";
		this->content_path = "./rss/error/413.html";
		this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
		this->connection = "Connection: Close\r\n";
		break;
	case 500: //  Internal Server Error
		this->code_description = " Internal Server Error\r\n";
		this->content_path = "./rss/error/500.html";
		this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
		this->connection = "Connection: Close\r\n";
		break;
	case 501: //  Not Implemented
		this->code_description = " Not Implemented\r\n";
		this->content_path = "./rss/error/501.html";
		this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
		break;
	case 505: //  HTTP Version Not Supported
		this->code_description = " HTTP Version Not Supported\r\n";
		this->content_path = "./rss/error/505.html";
		this->contentType = "Content-Type: text/html; charset=utf-8\r\n";
		this->connection = "Connection: Close\r\n";
		break;
	default:
		break;
	}
}

std::string Response::makeAutoindexPage(const char *path, std::string const &host) {
    std::string dirName(path);
	std::cout << "dirName: " << dirName << std::endl;
    size_t pos_index = dirName.rfind("index.");
    if (pos_index != std::string::npos)
	{
        dirName.erase(pos_index, dirName.length() - pos_index);
	}
    DIR *dir = opendir(dirName.c_str());
	// std::cout << "dirName2: " << dirName << std::endl;
	// std::cout << "dir: " << dir << std::endl;
    std::string page =\
    "<!DOCTYPE html>\n\
    <html>\n\
    <head>\n\
            <title>" + dirName + "</title>\n\
    </head>\n\
    <body>\n\
    <h1>INDEX</h1>\n\
    <p>\n";
    if (dir == NULL) 
	{
        std::cerr << "Error: could not open [" << path << "]" << std::endl;
        return "";
    }
    if (dirName[0] != '/')
        dirName = "/" + dirName;
    for (struct dirent *dirEntry = readdir(dir); dirEntry; dirEntry = readdir(dir)) 
	{
        std::stringstream   ss;
        struct stat vStat;
        stat(dirEntry->d_name, &vStat);
        ss << "\t\t<p><a href=\"http://" + host + dirName + dirEntry->d_name + "\">" + dirEntry->d_name + "</a></p>\n";
        page += ss.str();
    }
    page +="\
    </p>\n\
    </body>\n\
    </html>\n";
    closedir(dir);
    return page;
}

std::string check_ext(Request *request)
{
	std::string ext;
	std::string cont_type = "Content-Type: text/html;\r\n";
	size_t pos = request->getUri().find_last_of(".");
	if (pos != std::string::npos)
		ext = request->getUri().substr(pos + 1);
	std::cout << "EXT " << ext;
	if (ext == "png " || ext == "ico " || ext == "jpeg " || ext == "gif "\
		|| ext == "tiff " || ext == "x-icon " || ext == "svg+xml ")
		cont_type = "Content-Type: image/" + ext + ";\r\n";
	if (ext == "mpeg " || ext == "mp4 " || ext == "quicktime " || ext == "x-flv "\
		|| ext == "x-ms-wmv " || ext == "webm ")
		cont_type = "Content-Type: video/" + ext + ";\r\n";
	if (ext == "css " || ext == "csv " || ext == "html " || ext == "plain " \
		|| ext == "xml " /*|| ext == "php " || ext == "py "*/)
		cont_type = "Content-Type: text/" + ext + ";\r\n";
	return cont_type;
}

void Response::make_response(Request *request, Configuration *config)
{
	this->date = get_date() + "\r\n";
	this->status_code = request->getCode();
	if (this->status_code != 200)
		check_errors(this->status_code);
	else
	{
		check_method(config, request);
		check_errors(this->status_code);
	}
	// формируем ответ

	this->contentType = check_ext(request);
	std::stringstream response;
	std::stringstream content;
	if (request->getCGI())
	{
		// std:: stringstream response;
		CgiProcess cgi(*request, *this);
		cgi.execCGI(getContentPath(*config, request->getUri()));
		check_errors(cgi.getStatus());
		content << cgi.getBody();
		response << this->version << this->status_code << this->code_description
			<< this->date << this->server << this->connection << this->allow_method
			<< this->contentType << "Content-Length: " << content.str().length() << "\r\n"
			<< this->setCookie << "\r\n\r\n" << content.str();
	}
	else /*if (check_ext(request) == true)*/
	{
		// std::cout << "CONTENT PATH FOR AI: " << this->content_path << std::endl;
		std::ifstream file(this->content_path.c_str(), std::ios::in | std::ios::binary);
		// std::cout << "#################" << this->content_path << std::endl;
		if (!file.is_open())
		{
			if (config->getAutoindex())
			{
				// std::cout << "CONTENT PATH FOR AI: " << this->content_path << std::endl;
				std::string ret;
				ret = makeAutoindexPage(this->content_path.c_str(), config->getHost());
				// std::cout << "ret: " << ret << std::endl;
				content << ret;
			}
			else
			{
				this->status_code = 404;
				this->code_description = " Not Found\r\n";
				this->connection = "Connection: Close\r\n";
				file.open("./rss/error/404.html");
			}
		}
		std::vector<char> contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		response << this->version << this->status_code << this->code_description
			<< this->date << this->server << this->connection << this->allow_method
			<< this->contentType << "Content-Length: " << contents.size()
			<< "\r\n\r\n"; 
		response.write(contents.data(), contents.size()); // бинарник
		file.close();
	}
	// else /* if (check_ext(request) == false)*/
	// {
	// 	std::ifstream	file;


	// 	// std:: stringstream response;
	// 	// std::stringstream content;
	// 	file.open(this->content_path.c_str());
	// 	if (!file.is_open())
	// 	{
	// 		if (config->getAutoindex())
	// 		{
	// 			std::string ret;
	// 			ret = makeAutoindexPage(this->content_path.c_str(), config->getHost());
	// 			content << ret;
	// 		}
	// 		else
	// 		{
	// 			this->status_code = 404;
	// 			this->code_description = " Not Found\r\n";
	// 			this->connection = "Connection: Close\r\n";
	// 			file.open("./rss/error/404.html");
	// 		}
	// 	}
	// 	content << file.rdbuf();
	// 	file.close();
	// }
	// response << this->version << this->status_code << this->code_description
	// << this->date << this->server << this->connection << this->allow_method
	// << this->contentType << "Content-Length: " << content.str().length() << "\r\n"
	// << this->setCookie << "\r\n\r\n" << content.str().c_str();

	


	int send_res = 1;
	std::string resp = response.str();
	while (send_res > 0)
	{
		std::cout << "SEND_RES " << send_res << std::endl;
		send_res = send(fd, resp.c_str(), resp.size(), 0);
		if(send_res < 0)
			break;
		resp = resp.substr(send_res);
	}
	
}

// std::string Response::getContentPath(std::string uri) const
// {
// 	if (uri == "/home ")
// 		return ("./rss/home/index.html");
// 	if (uri.find("php") != std::string::npos)
// 		return ("./cgi/phpinfo.php");
// 	if (uri.find("cpp") != std::string::npos)
// 		return ("./cgi/print_env");
// 	return("./rss/home/index.html");
// }

std::string Response::getContentPath(Configuration conf, std::string uri)
{
	std::string contentPath;
	std::string uri_part;
	size_t lengthLocation = 0;

	// std::cout << "URI: " << uri << std::endl;
	int pos = uri.find_first_of('/', 1);
	uri_part = uri.substr(0, pos);
	// if (uri_part == "/home")
	// 	uri_part = "/ ";
	if (uri_part == "/rss")
	{
		uri = uri.substr(4, uri.length() - 4);
		contentPath = getContentPath(conf, uri);
		return contentPath;
	}
	// std::cout << "URI_PART: " << uri_part << std::endl;
	std::vector<location> array = conf.getArray();

	std::vector<location>::const_iterator it = array.begin();
	while(it != array.end())
	{
		
		if (uri_part.find(it->location) != std::string::npos)
		{
			if (lengthLocation < it->location.length())
			{
				contentPath = "." + it->root + uri;
				// std::cout << "contentPath befor open: [" << contentPath << "]" << std::endl;
				if (contentPath[contentPath.length() - 1] == ' ')
					contentPath = contentPath.substr(0, contentPath.length() - 1);
				int checkDir = open(contentPath.c_str(), O_DIRECTORY);
				// std::cout << "checkOpen: " << checkDir << std::endl;
				if (checkDir != -1)
				{
					if (it->location == "/")
						contentPath = contentPath + "/index.html";
					else
						contentPath = it->index;
					close(checkDir);
				}
				// else
				// {
				// 	int checkFile = open(contentPath.c_str(), O_RDONLY);
				// 	if (checkFile == -1)
				// 		this->status_code = 404;
				// 	else
				// 		close(checkFile);
				// }
			}
		}
		lengthLocation = it->location.length();
		it++;
	}
	if (it == array.end() && contentPath.empty()) 
		this->status_code = 404;
	// int checkFile = open(contentPath.c_str(), O_RDONLY);
	// if (checkFile == -1)
	// 	this->status_code = 404;
	std::cout << "contentPath: " << contentPath << std::endl;
	return contentPath;
}

void Response::check_method(Configuration *configs, Request *request)
{
	std::string uri_str = request->getUri();
	if (uri_str == "/ ")
		uri_str = "/home ";
	if (uri_str == "/redirect ")
		this->status_code = 301;
	std::string 	method[3] = {"GET", "POST", "DELETE"};
	int m = 0;
	while(m < 3 && request->getMethod().compare(method[m]) != 0 )
		m++;
	switch (m)
	{
	case 0: 	//		std::cout << "Method GET" << std::endl;
		if (configs->checkGet())
		{
			this->server = "Server: " + configs->getServerName() + "\r\n";
			if(request->getCGI())
				break;
			// this->content_path = configs->getIndex();
			this->content_path = getContentPath(*configs, uri_str);
			// std::cout << "CONTENT_PATH: " << this->content_path << std::endl;
			if (this->setCookie == "")
				this->setCookie = "Set-Cookie: name=" + configs->getServerName() + "\r\n";
		}	
		else
		{
			this->status_code = 405;
			this->allow_method = "Allow: " + configs->getHttpMethod() + "\r\n";
		}
		break;
	case 1:		//	std::cout << "Method POST" << std::endl;
			if (configs->checkPost())
			{
				this->server = "Server: " + configs->getServerName() + "\r\n";
				if(request->getBody().size() > (size_t) configs->getClientBodySize())
				{	
					this->status_code = 413;
					break;
				}
				if(request->getCGI())
					break;
				if(request->getPostFile() == true)
				{
					std::string content = request->getBody();
					std::string filename = "./rss/upload/" + content.substr((content.find("filename=\"") + 10));
					filename = filename.substr(0, filename.find("\""));
					size_t pos_beg = content.find("\r\n\r\n") + 4;
					size_t pos_end = content.find(request->boundary + "--\r\n") - 2;
					content = content.substr(pos_beg, pos_end - pos_beg);
					std::ofstream newfile;
					newfile.open(filename.c_str(),  std::ios_base::out | std::ios_base::binary);
					if (!newfile.is_open())
					{
						this->status_code = 500;
						break;
					}
					newfile << content;
					newfile.close();
					this->status_code = 201;
					this->code_description = " Created.\r\n";
					this->_location = "./rss/upload";
					this->connection = "Connection: Close\r\n";
					this->content_path = "./rss/error/201.html";
				}
				else
				{
					std::fstream newfile;
					newfile.open("./rss/post/test_post.txt", std::ios_base::app);
					if (!newfile.is_open())
					{
						this->status_code = 500;
						break;
					}
					newfile << request->getBody();
					this->status_code = 201;
					this->code_description = " Created.\r\n";
					this->_location = "./rss/post/test_post.txt";
					this->connection = "Connection: Close\r\n";
					newfile.close();
				}
				break;
			}	
			else
			{
				this->status_code = 405;
				this->allow_method = "Allow: " + configs->getHttpMethod() + "\r\n";
			}
			break;
	case 2: // std::cout << "Method DELETE" << std::endl;
		if (configs->checkDelete())
		{
			this->server = "Server: " + configs->getServerName() + "\r\n";
			this->content_path = getContentPath(*configs, uri_str);
			// this->content_path = uri_str.insert(0, std::string("./rss"));
			// for(size_t i = 0; i < this->content_path.length(); i++)
			// {
			// 	if(this->content_path[i] == ' ')
			// 	{
			// 		this->content_path.erase(i,1);
			// 		i--;
			// 	}
			// }
			if (remove(this->content_path.c_str()))
			{
				this->status_code = 500;
				std::cout << "Error in remove DELETE method." << std::endl;
			}
		}	
		else
		{
			this->status_code = 405;
			this->allow_method = "Allow: " + configs->getHttpMethod() + "\r\n";
		}
		break;
	default: //std::cout << "Uncknown method" << std::endl;	//501 not implemented
		this->status_code = 501;
		break;
	}	
}

Response::~Response()
{
}

// getters

int Response::getStatus_code() const
{
	return this->status_code;
}

std::string Response::getCodeDescription() const
{
	return this->code_description;
}

std::string Response::getVersion() const
{
	return this->version;
}

std::string Response::getServer() const
{
	return this->server;
}

std::string Response::getDate() const
{
	return this->date;
}

std::string Response::getAllow_method() const
{
	return this->allow_method;
}


std::ostream& operator<<(std::ostream& out, const Response& resp)
{
	out << "\033[33mResponse: \033[0m";
	out << "Status code: " << resp.getStatus_code() << " " << resp.getCodeDescription();
	out << "HTTP version: " << resp.getVersion() << std::endl;
	out << resp.getServer();
	out << "Date: " << resp.getDate();
	return (out);
}