#include "other_function.hpp"

class HttpResponse {
    public:
        HttpResponse(HttpRequest req){
            createRes(req);
        }
        std::string res;
        void createRes(HttpRequest req){
            std::string pathname = req.header["pathname"];
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
                std::cout << tmp.str() << "\n";
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
            else if(pathname == "/mes_board.html"){
                std::string html = other::myReadFile("mes_board.html");
                std::string data = other::myReadFile("board_content.txt");
                std::vector<std::string> data_vec = other::split(data, "\n");
                for(int i = 0 ; i < data_vec.size(); i++){
                    std::cout << "data_vec = " << data_vec[i] << "\n";
                }
                std::string data_final;
                for(int i = 0; i < data_vec.size(); i++){
                    data_final += "<li> " + other::parseBoardData(data_vec[i])+"</li><br>\n";
                }
                html.replace(html.find("<!--endMsg-->"), 13, data_final);
                std::stringstream tmp;
                tmp << "HTTP/1.1 200 OK\r\n" 
                    << "Content-Type: text/html\r\n"
                    << "Connection: close\r\n"
                    << "Content-Length: " << html.size() << "\r\n"
                    << "\r\n"
                    << html;
                res = tmp.str();
                // std::cout << tmp.str() << "\n";
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