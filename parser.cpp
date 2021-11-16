#include "Configuration.hpp"
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

std::vector<std::string> split_line(std::string line)
{
    std::vector<std::string> servers;
    size_t prev = 0, pos = 0;
    do
    {
        pos = line.find("server:", prev);
        if (pos == std::string::npos)
			pos = line.length();
        std::string serv = line.substr(prev, pos - prev);
        if (!serv.empty())
			servers.push_back(serv);
        prev = pos + 7;
    }
    while (pos < line.length() && prev < line.length());
    return servers;
}

int	read_conf(const char *path , std::vector<Configuration> &config)
{
	std::ifstream	file;
	std::string		line;
	// std::string		tmpline;
	std::vector<std::string>	servers;
	int 			serv_count = 0;
	file.open(path);
	if (!file.is_open())
		throw std::string("\033[31mThe configuration file could not be opened\033[0m");
	else if (file.peek() == EOF)
		throw std::string("\033[31mThe configuration file is empty\033[0m");
	std::stringstream content;
    // std::string      tmpline;

	content << file.rdbuf();   //содержимое файла переписываем в контент для дальнейшей работы с ним. файл менять не можем
    // tmpline = content.str();

	// while (std::getline(file, line))
	// 	tmpline += line;
	servers = split_line(content.str());		// делим конфиг файл на блоки - каждый сервер записываем в отдельный стринг для дальнейшего распарсивания строки, для каждого серв создается отдельный конф
	Configuration	conf;
	for (std::vector<std::string>::iterator it = servers.begin(); it != servers.end(); ++it)
	{
		line = *it;
		// std::cout << line << std::endl;
		// Configuration	conf;
		size_t pos_beg = 0;
		size_t pos_end = 0;
		while (1)
		{

			if (line.find("Host ", pos_beg) != std::string::npos)
			{
				pos_beg = line.find("Host ", pos_beg) + 5;
				pos_end = line.find(";", pos_beg);
				conf.setHost(line.substr(pos_beg, pos_end - pos_beg));
			}
			else if (line.find("Port ", pos_beg) != std::string::npos)
			{
				pos_beg = line.find("Port") + 5;
				pos_end = line.find(";", pos_beg);
				conf.setPort(line.substr(pos_beg, pos_end - pos_beg));
			}
			else if (line.find("server_name ", pos_beg) != std::string::npos)
			{
				pos_beg = line.find("server_name ") + 12;
				pos_end = line.find(";", pos_beg);
				conf.setServerName(line.substr(pos_beg, pos_end - pos_beg));
			}
			else if (line.find("default_error_pages ", pos_beg) != std::string::npos)
			{
				pos_beg = line.find("default_error_pages ") + 20;
				pos_end = line.find(";", pos_beg);
				conf.setDefaultErrorPages(line.substr(pos_beg, pos_end - pos_beg));
			}
			else if (line.find("client_body_size ", pos_beg) != std::string::npos)
			{
				pos_beg = line.find("client_body_size ") + 17;
				pos_end = line.find(";", pos_beg);
				conf.setClientBodySize(line.substr(pos_beg, pos_end - pos_beg));
			}
			else if (line.find("http_method ", pos_beg) != std::string::npos)
			{
				pos_beg = line.find("http_method ") + 12;
				pos_end = line.find(";", pos_beg);
				// std::cout << line.substr(pos_beg, pos_end - pos_beg) << "test" << std::endl;
				conf.setHttpMethod(line.substr(pos_beg, pos_end - pos_beg));
			}
			else
				break;
		}
		config.push_back(conf);
		serv_count++;
	}
	file.close();
	return serv_count;
}
