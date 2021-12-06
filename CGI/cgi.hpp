#ifndef CGI_HPP
#define CGI_HPP

#include "../inc/webserv.hpp"
// #include "Request.hpp"
#include <unistd.h>

class Request;

class CGI
{
    private:
        std::map<std::string, std::string> env;
        std::string body;
    public:
        CGI(Request req);
        ~CGI();
        void cgi_main(void);
        char** map_to_array();
        std::string getQueryString(std::string URI);
};


#endif