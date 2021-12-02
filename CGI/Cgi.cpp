#include "Cgi.hpp"

CGI::CGI(Request req)
{
    // REQUEST_METHOD +
    // REQUEST_URI +
    // QUERY_STRING +
    // REMOTE_ADDR
    // SCRIPT_NAME
    // HTTP_COOKIE
    // HTTP_REFERER

    //content-type = chunk (content-lenght) - раз
    this->env["REQUEST_METHOD"] = req.getMethod();
    this->env["REQUEST_URI"] = req.getUri();
    this->env["QUERY_STRING"] = getQueryString(this->env["REQUEST_URI"]);
    this->env["GATEWAY_INTERFACE"] = "CGI/1.1";

}

std::string CGI::getQueryString(std::string URI)
{
    size_t pos = 0;
    std::string QueryString;

    while(pos != URI.find("?", 0))
			pos++;
    QueryString = URI.substr(pos, URI.length() - pos);
    return QueryString;
}

CGI::~CGI()
{

}

void CGI::cgi_main(void)
{
    pid_t pid;
    int status;

    switch(pid=fork())
    {
        case -1: 
        {
            std::cerr << "Process creation failed" << std::endl;
        }
        case 0 : 
        {
            // execve(name_of_script, NULL, env) - выполнение скрипта cgi
        }
        default : 
        {
            waitpid(pid, &status, 0);
        }
    }
}


// this->env["REQUEST_METHOD"] = req.getMethod();
    // //здесь нужна map для заголовоков из запроса
    // std::map<std::string, std::string> headers;
    // if (headers.find("Authorization") != headers.end())
    // {
    //     this->env["AUTH_TYPE"] = headers["Authorization"];
    //     // if (this->env["AUTH_TYPE"] != "Basic" && this->env["AUTH_TYPE"] != "Digest"
    //     //     && this->env["AUTH_TYPE"] != "NTLM" && this->env["AUTH_TYPE"] != "Negotiate")
    //     // {
    //     //     std::cout << "You have an errors AUTH_TYPE!";
    //     // }                                                    // нужна ли проверка  AUTH_TYPE ????
    // }

    // this->env["CONTENT_LENGTH"] = std::to_string(this->body.length());

    // if (this->env["REQUEST_METHOD"] == "POST")
    //     this->env["CONTENT_TYPE"] = headers["Content-type"];
    
    // this->env["GATEWAY_INTERFACE"] = "CGI/1.1";

    // // this->env["PATH_INFO"] =        //взять из request

    // // пример PATH_INFO:
    // // http://somehost.com/cgi-bin/somescript/this%2eis%2epath%3binfo

    // // /this.is.the.path;info


    // // this->env["PATH_TRANSLATED"] =     //взять из request


    // // this->env["QUERY_STRING"] =            //взять из request

    // this->env["REMOTE_ADDR"] = headers["HTTP_X_FORWARDED_FOR"];

    // this->env["REMOTE_HOST"] = req.getHost();                  //под вопросом 

    // this->env["REMOTE_IDENT"] = ;

    // this->env["REMOTE_USER"] = ;

    


    // this->env["SERVER_SOFTWARE"] = ; // из конфига
    // this->env["SERVER_NAME"] = ; //из конфига
    // this->env["SERVER_PORT"] = ; //из конфига
    
    
//    SERVER_SOFTWARE = Apache/2.0.28 (Win32) 
//    SERVER_NAME = localhost
//    SERVER_PROTOCOL = HTTP/1.1 
//    SERVER_PORT = 80 


//    SERVER_ADMIN = mike@home 
//    GATEWAY_INTERFACE = CGI/1.1 
//    DOCUMENT_ROOT = C:/Apache2/htdocs 
   
//    REQUEST_METHOD = GET 
//    SCRIPT_FILENAME = C:/Apache2/htdocs/App/showEnv.exe 
//    SCRIPT_NAME = /app/showEnv.exe 
//    CONTENT_TYPE = 
//    CONTENT_LENGTH = 
//    QUERY_STRING = 
//    REQUEST_URI = /app/showEnv.exe 
//    PATH_INFO = 
//    PATH_TRANSLATED = 
    
//    AUTH_TYPE = 
//    REMOTE_HOST = 
//    REMOTE_ADDR = 127.0.0.1 
//    REMOTE_USER = 
   
//    HTTP_HOST = localhost 
//    HTTP_ACCEPT_CHARSET = 
//    HTTP_ACCEPT_LANGUAGE = en-us 
//    HTTP_CONNECTION = Keep-Alive 
//    HTTP_REFERER = 
//    HTTP_USER_AGENT = Mozilla/4.0 (compatible; MSIE 5.5; Windows NT 5.0; DigExt)
    
    // std::cout << this->env["REQUEST_METHOD"] << std::endl;
    // std::cout << this->env["QUERY_STRING"] << std::endl;
    // std::cout << this->env["SERVER_PROTOCOL"] << std::endl;