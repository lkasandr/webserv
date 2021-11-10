#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "./socket/Socket.hpp"
#include "./111/Configuration.hpp"
#include <cstdlib>
#include <unistd.h>
#include <cerrno>
#include <vector>

std::vector<std::string> split_line(std::string line);
int	read_conf(char *path , std::vector<Configuration> &config);

#endif
