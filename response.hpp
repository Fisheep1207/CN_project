#include "other_function.hpp"

class HttpResponse {
    public:
        HttpResponse(std::string pathname){
            createRes(pathname);
        }
        std::string res;
        void createRes(std::string pathname){
            std::cout << "pathname = " << pathname << "\n";
            if(pathname == "/"){
                std::string html = other::myReadFile("menu.html");
                std::stringstream tmp;
                tmp << "HTTP/1.1 200 OK\r\n" 
                    << "Content-Type: text/html\r\n"
                    << "Connection: close\r\n"
                    << "Content-Length: " << html.size() << "\r\n"
                    << "\r\n"
                    << html;
                res = tmp.str();
            }
            else if (pathname == "/favicon.ico"){
                std::string image = other::myReadFile("patrick.png");
                std::stringstream tmp;
                tmp << "HTTP/1.1 200 OK\r\n" 
                    << "Content-Type: image/png\r\n"
                    << "Connection: close\r\n"
                    << "Content-Length: " << image.size() << "\r\n"
                    << "\r\n"
                    << image;
                res = tmp.str();
            }
            else if (pathname == "/patrick.png"){
                std::string image = other::myReadFile("patrick.png");
                std::stringstream tmp;
                tmp << "HTTP/1.1 200 OK\r\n" 
                    << "Content-Type: image/png\r\n"
                    << "Connection: close\r\n"
                    << "Content-Length: " << image.size() << "\r\n"
                    << "\r\n"
                    << image;
                res = tmp.str();
            }
            else{
                std::string html = other::myReadFile("404.html");
                std::stringstream tmp;
                tmp << "HTTP/1.1 404 Not Found\r\n" 
                    << "Content-Type: text/html\r\n"
                    << "Connection: close\r\n"
                    << "Content-Length: " << html.size() << "\r\n"
                    << "\r\n"
                    << html;
                res = tmp.str();
            }
            return;
        };
};