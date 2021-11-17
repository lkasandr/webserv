#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "Socket.hpp"
#include "Configuration.hpp"
#include <cstdlib>
#include <unistd.h>
#include <cerrno>
#include <vector>

// #include "Configuration.hpp"
// #include <vector>
#include <fstream>
#include <sstream>
#include <string>



std::vector<std::string> split_line(std::string line);
int	read_conf(const char *path , std::vector<Configuration> &config);

#endif
