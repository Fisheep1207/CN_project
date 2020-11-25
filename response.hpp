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
            if (pathname == "/register.html"){
                std::string html = other::myReadFile("./staticFile/register.html");
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
		        std::cout << "Entry => login.html\n";
		        if(req.header["method"] == "GET"){
                    std::cout << "Entry GET\n";
                    if(req.header.find("Cookie") == req.header.end()){  // 如果沒有 cookie
                        std::cout << "Entry GET Without Cookie\n";
                        std::string html = other::myReadFile("./staticFile/login.html");
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
                            // std::string html = other::myReadFile("menu.html");
                            std::stringstream tmp;
                            tmp << "HTTP/1.1 303 See Other\r\n" 
                                << "Content-Type: text/html\r\n"
                                << "Location: /\r\n"
                                // << "Content-Length: " << html.size() << "\r\n"
                                << "Set-Cookie: login="+cur_cookie+"\r\n" 
                                << "\r\n";
                                // << html;
                            res = tmp.str();
                        }
                        else{
                            std::cout << "Entry GET With wrong Cookie\n";
                            std::stringstream tmp;
                            tmp << "HTTP/1.1 303 See Other\r\n" 
                                << "Content-Type: text/html\r\n"
                                << "Location: /\r\n"
                                << "Set-Cookie: login=deleted; expires=Thu, 01 Jan 1970 00:00:00 GMT"
                                << "\r\n";
                            res = tmp.str();
                        }
                    }
                }
                else if (req.header["method"] =="POST"){
                    std::cout << "Entry POST\n";
                    std::map<std::string, std::string> user = other::getUsernameAndPassword(req);
                    int len = std::stoi(req.header["Content-Length"], nullptr);
                    if(req.raw_request.size() - len <= 0){
                        std::stringstream tmp;
                        tmp << "HTTP/1.1 303 See Other\r\n" 
                            << "Content-Type: text/html\r\n"
                            << "Location: /chromePLS.html\r\n"
                            << "Set-Cookie: login=deleted; expires=Thu, 01 Jan 1970 00:00:00 GMT"
                            << "\r\n";
                        res = tmp.str();
                    }
                    else if(user["action"] == "join"){   // redirect 才對
                        std::cout << "Entry POST join\n";
                        std::string new_cookie = other::generateUUID();
                        std::ofstream userFile;
                        std::ofstream passwordFile;
                        userFile.open("./cookie/"+new_cookie+".txt");
                        passwordFile.open("./password/"+user["username"]+".txt");
                        passwordFile << user["password"] << "\n";
                        userFile << user["username"] << "\n";
                        userFile << user["password"] << "\n";
                        userFile.close();
                        std::stringstream tmp;
                        tmp << "HTTP/1.1 303 See Other\r\n" 
                            << "Content-Type: text/html\r\n"
                            << "Location: /\r\n"
                            // << "Content-Length: " << html.size() << "\r\n"
                            << "Set-Cookie: login="+new_cookie+"\r\n" 
                            << "\r\n";
                            // << html;
                        res = tmp.str();
                    }
                    else if (user["action"] == "login"){
                        std::cout << "Entry POST login\n";
                        // if(req.header.find("Cookie") == req.header.end()){
                            std::cout << "Entry POST without Cookie (login)\n";
                            std::string user_pass_file = "./password/"+user["username"]+".txt";
                            std::ifstream f(user_pass_file.c_str());
                            if(f.good()){   // 如果有 user 的檔案 // redirect 才對
                                std::string password;
                                std::getline(f, password);
                                if(password == user["password"]){
                                    std::string new_cookie = other::generateUUID();
                                    std::ofstream userFile;
                                    userFile.open("./cookie/"+new_cookie+".txt");
                                    userFile << user["username"] << "\n";
                                    userFile << user["password"] << "\n";
                                    std::stringstream tmp;
                                    tmp << "HTTP/1.1 303 See Other\r\n" 
                                        << "Content-Type: text/html\r\n"
                                        << "Location: /\r\n"
                                        // << "Content-Length: " << html.size() << "\r\n"
                                        << "Set-Cookie: login="+new_cookie+"\r\n" 
                                        << "\r\n";
                                        // << html;
                                    res = tmp.str();
                                }
                                else{ // 要增加「密碼錯誤」那行
                                    std::stringstream tmp;
                                    tmp << "HTTP/1.1 303 See Other\r\n" 
                                        << "Content-Type: text/html\r\n"
                                        << "Location: /login.html\r\n"
                                        // << "Content-Length: " << html.size() << "\r\n"
                                        << "\r\n";
                                        // << html;
                                    res = tmp.str();
                                }
                            }
                            else{ // 帳號密碼錯誤 
                                std::string html = other::myReadFile("./staticFile/login.html");
                                std::stringstream tmp;
                                std::string wrong = "<!--Wrong Username or Password-->";
                                std::cout << html.find("<!--Wrong") << "\n";
                                html.replace(html.find("<!--Wrong"), wrong.size(), "<p>Wrong Username or Password</p>");
                                tmp << "HTTP/1.1 200 OK\r\n" 
                                    << "Content-Type: text/html\r\n"
                                    << "Connection: close\r\n"
                                    << "Content-Length: " << html.size() << "\r\n"
                                    << "\r\n"
                                    << html;
                                res = tmp.str();
                            }
                        // }
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
            else if (pathname == "/favicon.ico"){
                std::string image = other::myReadFile("./staticFile/patrick.png");
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
                std::string image = other::myReadFile("./staticFile/patrick.png");
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
                std::string image = other::myReadFile("./staticFile/spongeBob.png");
                std::stringstream tmp;
                tmp << "HTTP/1.1 200 OK\r\n" 
                    << "Content-Type: image/png\r\n"
                    << "Connection: close\r\n"
                    << "Content-Length: " << image.size() << "\r\n"
                    << "\r\n"
                    << image;
                res = tmp.str();
            }
            else if(pathname == "/chromePLS.html"){
                std::string html = other::myReadFile("./staticFile/chromePLS.html");
                std::stringstream tmp;
                tmp << "HTTP/1.1 200 OK\r\n" 
                    << "Content-Type: text/html\r\n"
                    << "Connection: close\r\n"
                    << "Content-Length: " << html.size() << "\r\n"
                    << "\r\n"
                    << html;
                res = tmp.str();
            }
            else if(req.header.find("Cookie") == req.header.end()){
                std::stringstream tmp;
                tmp << "HTTP/1.1 303 See Other\r\n" 
                    << "Content-Type: text/html\r\n"
                    << "Location: /login.html\r\n"
                    // << "Content-Length: " << html.size() << "\r\n"
                    << "\r\n";
                    // << html;
                res = tmp.str();
            }
            else if(pathname == "/"){
                std::string html = other::myReadFile("./staticFile/menu.html");
                std::string cur_cookie = req.header["Cookie"];
                cur_cookie = cur_cookie.substr(7);
                std::string cur_cookie_file = "./cookie/"+ cur_cookie +".txt";
                // std::cout << "fuck fuck fuck   " + cur_cookie_file << "\n";
                std::ifstream f(cur_cookie_file.c_str());
                if(f.good()){   // 如果有 cookie 的檔案
                    std::string username;
                    std::getline(f, username);
                    std::stringstream tmp;
                    html.replace(html.find(" Login "), 7, username);
                    html.replace(html.find("<!--Logout"), 10, " ");
                    html.replace(html.find("Logout-->"), 9, " ");
                    tmp << "HTTP/1.1 200 OK\r\n" 
                        << "Content-Type: text/html\r\n"
                        << "Connection: close\r\n"
                        << "Content-Length: " << html.size() << "\r\n"
                        << "\r\n"
                        << html;
                        // << html;
                    res = tmp.str();
                }
                else{    
                    std::stringstream tmp;
                    tmp << "HTTP/1.1 303 See Other\r\n" 
                        << "Content-Type: text/html\r\n"
                        << "Location: /login.html\r\n"
                        << "\r\n";
                    res = tmp.str();
                }
                //std::cout << tmp.str() << "\n";
            }
            else if(pathname == "/mes_board.html"){
                std::string cur_cookie = req.header["Cookie"];
                cur_cookie = cur_cookie.substr(7);
                std::string cur_cookie_file = "./cookie/"+ cur_cookie +".txt";
                std::ifstream f(cur_cookie_file.c_str()); 
                if(f.good()){   // 如果有 cookie 的檔案
                    std::string username;
                    std::getline(f, username);
                    std::string html = other::myReadFile("./staticFile/mes_board.html");
                    std::string data = other::myReadFile("./msgBoard/board_content.txt");
		            std::vector<std::string> data_vec = other::split(data, "\n");
                    if(req.header["method"] == "POST"){
                        int len = std::stoi(req.header["Content-Length"], nullptr);
                        if(req.raw_request.size() - len <= 0){
                            std::cout << "POST Without BODY!\n";
                            std::stringstream tmp;
                            tmp << "HTTP/1.1 303 See Other\r\n" 
                                << "Content-Type: text/html\r\n"
                                << "Location: /chromePLS.html\r\n"
                                << "Set-Cookie: login=deleted; expires=Thu, 01 Jan 1970 00:00:00 GMT"
                                << "\r\n";
                            res = tmp.str();
                            return;
                        }
                        else{
                            std::vector<std::string> body = other::split(req.raw_request, "\r\n\r\n");
                            int i_auto = std::stoi(req.header["Content-Length"], nullptr);
                            std::ofstream boardFile;
                            boardFile.open("./msgBoard/board_content.txt", std::ios::app);
                            if(!boardFile) std::cout << "Failed!!\n";
                            boardFile << "<li> " << body[1].substr(0, i_auto) << " </li>\n";
                            boardFile.close();
                            data_vec.push_back("<li> " + body[1].substr(0, i_auto) + " </li>\n");
                        }
                    }
                    std::string data_final;
                    for(int i = 0 ; i < data_vec.size(); i ++){
                        std::cout << "here in loop\n";
                        std::cout << data_vec[i] << "\n";	
                    }
                    for(int i = 0; i < data_vec.size(); i++){
                        data_final += "<li> " + other::parseBoardData(data_vec[i])+"</li><br>\n";
                    }
                    html.replace(html.find("<!--endMsg-->"), 13, data_final);
                    html.replace(html.find(" Login "), 7, username);
                    html.replace(html.find("<!--Logout"), 10, " ");
                    html.replace(html.find("Logout-->"), 9, " ");
                    std::stringstream tmp;
                    tmp << "HTTP/1.1 200 OK\r\n" 
                        << "Content-Type: text/html\r\n"
                        << "Connection: close\r\n"
                        << "Content-Length: " << html.size() << "\r\n"
                        << "\r\n"
                        << html;
                    res = tmp.str();
                }
                else{
                    std::stringstream tmp;
                    tmp << "HTTP/1.1 303 See Other\r\n" 
                        << "Content-Type: text/html\r\n"
                        << "Location: /login.html\r\n"
                        << "\r\n";
                    res = tmp.str();
                }
                // std::cout << tmp.str() << "\n";
            }
            else if(pathname == "/logout"){
                std::stringstream tmp;
                tmp << "HTTP/1.1 303 See Other\r\n" 
                    << "Content-Type: text/html\r\n"
                    << "Location: /login.html\r\n"
                    << "Set-Cookie: login=deleted; expires=Thu, 01 Jan 1970 00:00:00 GMT"
                    << "\r\n";
                res = tmp.str();
            }
            else{
                std::string html = other::myReadFile("./staticFile/404.html");
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
