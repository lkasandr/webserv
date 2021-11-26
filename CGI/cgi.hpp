#ifndef CGI_HPP
#define CGI_HPP

#include "../inc/webserv.hpp"
#include "../Request/Request.hpp"
#include <map>

class CGI
{
    private:
        std::map<std::string, std::string> env;
        std::string body;
    public:
        CGI(Request req);
        ~CGI();
        void cgi_main(void);
};


#endif