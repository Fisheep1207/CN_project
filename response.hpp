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
            else if (pathname == "/spongeBob.png"){
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
                // for(int i = 0 ; i < data_vec.size(); i++){
                //     std::cout << "data_vec = " << data_vec[i] << "\n";
                // }
                if(req.header["method"] == "POST"){
                    std::vector<std::string> body = other::split(req.raw_request, "\r\n\r\n");
                    int i_auto = std::stoi(req.header["Content-Length"], nullptr);
                    std::ofstream boardFile;
                    boardFile.open("board_content.txt", std::ios::app);
                    boardFile << "<li> " << body[1].substr(0, i_auto) << " </li>\n";
                    boardFile.close();
                    data_vec.push_back("<li> " + body[1].substr(0, i_auto) + " </li>\n");
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
            else if (pathname == "/register.html"){
                std::string html = other::myReadFile("register.html");
                std::stringstream tmp;
                tmp << "HTTP/1.1 200 OK\r\n" 
                    << "Content-Type: text/html\r\n"
                    << "Connection: close\r\n"
                    << "Content-Length: " << html.size() << "\r\n"
                    << "\r\n"
                    << html;
                res = tmp.str();
            }
            else if(pathname == "/login.html"){
                if(req.header["method"] == "GET"){
                    std::cout << "Entry GET\n";
                    if(req.header.find("Cookie") == req.header.end()){  // 如果沒有 cookie
                        std::cout << "Entry GET Without Cookie\n";
                        std::string html = other::myReadFile("login.html");
                        std::stringstream tmp;
                        tmp << "HTTP/1.1 200 OK\r\n" 
                            << "Content-Type: text/html\r\n"
                            << "Connection: close\r\n"
                            << "Content-Length: " << html.size() << "\r\n"
                            << "\r\n"
                            << html;
                        res = tmp.str();
                    }
                    else{   //如果有 cookie ，應該要用 redirect !!!
                        std::cout << "Entry GET Have Cookie\n";
                        std::string cur_cookie = req.header["Cookie"];
                        cur_cookie = cur_cookie.substr(7);
                        std::string cur_cookie_file = "./cookie/"+ cur_cookie +".txt";
                        // std::cout << "fuck fuck fuck   " + cur_cookie_file << "\n";
                        std::ifstream f(cur_cookie_file.c_str());
                        if(f.good()){   // 如果有 cookie 的檔案
                            std::string html = other::myReadFile("menu.html");
                            std::stringstream tmp;
                            tmp << "HTTP/1.1 200 OK\r\n" 
                                << "Content-Type: text/html\r\n"
                                << "Connection: close\r\n"
                                << "Content-Length: " << html.size() << "\r\n"
                                << "Set-Cookie: login="+cur_cookie+"\r\n" 
                                << "\r\n"
                                << html;
                            res = tmp.str();
                        }
                        else{   // 如果沒有 cookie 的檔案
                            std::cout << "Entry GET Without Cookie File\n";
                            std::string new_cookie = other::generateUUID();
                            std::ofstream userFile;
                            std::vector<std::string> body = other::split(req.raw_request, "\r\n\r\n");
                            int i_auto = std::stoi(req.header["Content-Length"], nullptr);
                            body[1] = body[1].substr(0, i_auto);
                            std::vector<std::string> tmp_body = other::split(body[1], "&");
                            userFile.open("./cookie/"+new_cookie+".txt");
                            std::string username = tmp_body[0].substr(tmp_body[0].find("="));
                            std::string password = tmp_body[1].substr(tmp_body[1].find("="));
                            userFile << username << "\n";
                            userFile << password << "\n";
                            userFile.close();
                            std::string html = other::myReadFile("menu.html");
                            std::stringstream tmp;
                            tmp << "HTTP/1.1 200 OK\r\n" 
                                << "Content-Type: text/html\r\n"
                                << "Connection: close\r\n"
                                << "Set-Cookie: login="+new_cookie+"\r\n"
                                << "Content-Length: " << html.size() << "\r\n"
                                << "\r\n"
                                << html;
                            res = tmp.str();
                        }
                    }
                }
                else if (req.header["method"] =="POST"){
                    std::cout << "Entry POST\n";
                    if(req.header.find("Cookie") == req.header.end()){
                        std::cout << "Entry POST without Cookie\n";
                        std::string new_cookie = other::generateUUID();
                        std::ofstream userFile;
                        std::vector<std::string> body = other::split(req.raw_request, "\r\n\r\n");
                        int i_auto = std::stoi(req.header["Content-Length"], nullptr);
                        body[1] = body[1].substr(0, i_auto);
                        std::vector<std::string> tmp_body = other::split(body[1], "&");
                        userFile.open("./cookie/"+new_cookie+".txt");
                        std::string username = tmp_body[0].substr(tmp_body[0].find("="));
                        std::string password = tmp_body[1].substr(tmp_body[1].find("="));
                        userFile << username << "\n";
                        userFile << password << "\n";
                        userFile.close();
                        std::string html = other::myReadFile("menu.html");
                        std::stringstream tmp;
                        tmp << "HTTP/1.1 200 OK\r\n" 
                            << "Content-Type: text/html\r\n"
                            << "Connection: close\r\n"
                            << "Set-Cookie: login="+new_cookie+"\r\n"
                            << "Content-Length: " << html.size() << "\r\n"
                            << "\r\n"
                            << html;
                        res = tmp.str();
                    }
                    // else{  // 這個只有在想要改密碼的時候會用到
                    //     std::string cur_cookie = req.header.find("Cookie");
                    //     std::cout << "cur_cookie =" << cur_cookie << "\n";
                    //     std::string data = other::myReadFile("./cookie/"+cur_cookie+".txt");
                    //     std::vector<std::string> try_user = other::split(data, "\n");
                    //     std::vector<std::string> body = other::split(req.raw_request, "\r\n\r\n");
                    //     int i_auto = std::stoi(req.header["Content-Length"], nullptr);
                    //     body[1] = body[1].substr(0, i_auto);
                    //     std::vector<std::string> tmp = other::split(body[1], "&");
                    //     userFile.open("./cookie/"+new_cookie+".txt");
                    //     std::string username = tmp[0].substr(tmp[0].find("="));
                    //     std::string password = tmp[1].substr(tmp[1].find("="));
                    //     if(try_user[0] == username && try_user[1] == password){
                    //         tmp << "HTTP/1.1 200 OK\r\n" 
                    //             << "Content-Type: text/html\r\n"
                    //             << "Connection: close\r\n"
                    //             << "Content-Length: " << html.size() << "\r\n"
                    //             << "\r\n"
                    //             << html;
                    //         res = tmp.str();
                    //     }
                    // }
                }
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