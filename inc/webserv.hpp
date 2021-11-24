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

#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read
#include <cerrno>

#include "Server.hpp"
#include "Configuration.hpp"
#include "Request.hpp"
#include "Socket.hpp"
#include "webserv.hpp"
#include <vector>
#include <poll.h>




std::vector<std::string> split_line(std::string line);
int	read_conf(const char *path , std::vector<Configuration> &config);

#endif
