#include "Configuration.hpp"
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

#include <ctype.h>

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

std::string get_key(std::string line)
{
	int i = 0;
	int j = 0;

	while (line[i] && line[i] != ' ')
		i++;
	while (line[j] && line[j] != ';')
		j++;
	return (line.substr(i + 1, j - i - 1));
}

void add_config(std::string line, Configuration& conf)
{
	if (line.find("Host ", 0) != std::string::npos)
		conf.setHost(get_key(line));
	else if (line.find("Port ", 0) != std::string::npos)
		conf.setPort(get_key(line));
	else if (line.find("autoindex on", 0) != std::string::npos)
		conf.setAutoindexOn();
	else if (line.find("server_name ", 0) != std::string::npos)
		conf.setServerName(get_key(line));
	// else if (line.find("location ", 0) != std::string::npos)
	// 	conf.setLocation(get_key(line));
	// else if (line.find("root ", 0) != std::string::npos)
	// {
	// 	conf.setRoot(get_key(line));
	// 	conf.setPath(conf.getLocation(), conf.getRoot());
	// }
	// else if (line.find("index ", 0) != std::string::npos)
	// 	conf.setIndex(get_key(line));
	else if (line.find("default_error_pages ", 0) != std::string::npos)
		conf.setDefaultErrorPages(get_key(line));
	else if (line.find("client_body_size ", 0) != std::string::npos)
		conf.setClientBodySize(get_key(line));
	else if (line.find("http_method ", 0) != std::string::npos)
		conf.setHttpMethod(get_key(line));
	else if (line.find("cgi_pass ", 0) != std::string::npos)
		conf.setCGI(get_key(line));
}

//Добавляет location в структуру
void add_location(std::string line, Configuration& conf)
{
	std::string location;
	std::string root;
	std::string index;
	size_t pos = 0;
	size_t pos_temp = 0;

	while(pos != line.length())
	{
		if (pos == line.find("location ", 0))
		{
			pos = pos + 9;
			pos_temp = pos;
			while ((pos != line.find(' ', 11)) && (pos != line.find('\n', 11)) && (pos != line.find('{', 11)))
				pos++;
			location = line.substr(pos_temp, pos - pos_temp);
			line = line.substr(pos, line.length() - pos);
			// std::cout << "LOcATION: " << location << std::endl;
			// std::cout << "LINE: " << line << std::endl;
			pos = -1;
		}
		else if (pos == line.find("root ", 0))
		{
			pos = pos + 5;
			pos_temp = pos;
			// std::cout << "POS: " << pos << std::endl;
			// std::cout << "LINE: " << line << std::endl;
			while (pos != line.find(';', pos_temp))
				pos++;
			root = line.substr(pos_temp, pos - pos_temp);
			// std::cout << "root: " << root << std::endl;
			line = line.substr(pos, line.length() - pos);
			pos = -1;
		}
		else if (pos == line.find("index ", 0))
		{
			pos = pos + 6;
			pos_temp = pos;
			while (pos != line.find(';', pos_temp))
				pos++;
			index = line.substr(pos_temp, pos - pos_temp);
			// std::cout << "index: " << index << std::endl;
			line = line.substr(pos, line.length() - pos);
			pos = -1;
		}
		pos++;
	}
	conf.setArray(location, root, index);
}

int	read_conf(const char *path , std::vector<Configuration> &config)
{
	std::ifstream	file;
	std::string		line;
	std::vector<std::string>	servers;
	int 			serv_count = 0;
	file.open(path);
	if (!file.is_open())
		throw std::string("\033[31mThe configuration file could not be opened\033[0m");
	else if (file.peek() == EOF)
		throw std::string("\033[31mThe configuration file is empty\033[0m");
	std::stringstream content;
	content << file.rdbuf();   //содержимое файла переписываем в контент для дальнейшей работы с ним. файл менять не можем
	servers = split_line(content.str());		// делим конфиг файл на блоки - каждый сервер записываем в отдельный стринг для дальнейшего распарсивания строки, для каждого серв создается отдельный конф
	// Configuration	conf;
	for (std::vector<std::string>::iterator it = servers.begin(); it != servers.end(); ++it)
	{
		line = *it;
		Configuration	conf;
		size_t pos = 0;
		std::string temp;

		std::string location;

		while(pos < line.length())
		{
			if (pos == line.find("location "))
			{
				while (pos != line.find("}"))
					pos++;
				location = line.substr(0, pos);
				line = line.substr(pos + 1, line.length());
				add_location(location, conf);
				pos = 0;
			}
			if (pos == line.find("\n"))
			{
				temp = line.substr(0, pos);
				line = line.substr(pos + 1, line.length());
				add_config(temp, conf);
				pos = -1;
			}
			pos++;
		}
		// conf.getPath();
		config.push_back(conf);
		serv_count++;
	}
	file.close();
	return serv_count;
}
