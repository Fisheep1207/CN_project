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
                                std::cout << ("<!--Wrong Username or Password-->");
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
            else if (pathname == "/profile1.jpg"){
                std::string image = other::myReadFile("./staticFile/profile1.jpg");
                std::stringstream tmp;
                tmp << "HTTP/1.1 200 OK\r\n" 
                    << "Content-Type: image/jpg\r\n"
                    << "Connection: close\r\n"
                    << "Content-Length: " << image.size() << "\r\n"
                    << "\r\n"
                    << image;
                res = tmp.str();
            }
            else if (pathname == "/profile2.jpg"){
                std::string image = other::myReadFile("./staticFile/profile2.jpg");
                std::stringstream tmp;
                tmp << "HTTP/1.1 200 OK\r\n" 
                    << "Content-Type: image/jpg\r\n"
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
                //std::cout << "fuck fuck fuck = " << cur_cookie_file << "\n";
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
                    // for(int i = 0 ; i < data_vec.size(); i ++){
                    //     std::cout << "here in loop\n";
                    //     std::cout << data_vec[i] << "\n";	
                    // }
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
            else if (pathname == "/video1.html"){
                std::string html = other::myReadFile("./staticFile/video1.html");
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
            else if (pathname == "/video2.html"){
                std::string html = other::myReadFile("./staticFile/video2.html");
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
            else if(pathname == "/1.mp4"){
                long long fileSize = other::readFileSize("./video/1.mp4");
                std::string range = req.header["Range"];
                if(range != ""){
                    // std::cout << "range = " << range << "\n";
                    range = range.replace(range.find("bytes="), 6, "");
                    std::vector<std::string> range_vec = other::split(range, "-");
                    long long int start = std::stoi(range_vec[0], nullptr);
                    // std::cout << "here12345!!!\n";        
	    	    //std::cout << "vec0 = "<< range_vec[0]<< " vec2 = "<< range_vec[1]<< " boolean = "<< (range_vec[1] == "") << "\n";
                    long long int end = fileSize;
                    if(range_vec.size() >= 2){
                        end = range_vec[1]!=""? std::stoi(range_vec[1], nullptr): fileSize-1;
                    }
                    // std::cout << "here444!!!\n";
                    long long int chunksize = (end-start)+1;
                    if (chunksize >= 200000){
                        chunksize = 200000;
                        end = chunksize-1+start;
                    }
                    std::string data = other::myReadFileWithSize("./video/1.mp4",start, chunksize);
                    if(start+chunksize >= fileSize) end = fileSize-1;
                    // std::cout << "fuck bro" << data << "\n";
                    std::stringstream tmp;
                    // std::cout << "here123!!!\n";
                    tmp << "HTTP/1.1 206 Partial Content\r\n"
                        << "Content-Type: video/mp4\r\n" \
                        << "Accept-Ranges: bytes\r\n" \
                        << "Connection: keep-alive\r\n" \
                        << "Content-Length: "<<  std::to_string(chunksize) <<  "\r\n" \
                        << "Content-Range: " << "bytes "+std::to_string(start)<<"-"+std::to_string(end)<<"/"+std::to_string(fileSize)<<"\r\n" \
                        << "\r\n" 
                        << data;
                    std::cout << "here!!!\n";
                    res = tmp.str();
                }
                else{
                    // std::string data = other::myReadFile("./video/1.mp4");
                    // std::stringstream tmp;
                    std::string data = other::myReadFileWithSize("./video/1.mp4", 0, 200000);
                    std::stringstream tmp;
                    tmp << "HTTP/1.1 200 OK\r\n"
                        << "Content-Type: video/mp4\r\n" \
                        << "Connection: keep-alive\r\n" \
                        << "Content-Length: "<< 200000 << "\r\n" \
                        << "\r\n"
                        << data;
                    res = tmp.str();
                }
            }
            else if(pathname == "/2.mp4"){
                long long fileSize = other::readFileSize("./video/2.mp4");
                std::string range = req.header["Range"];
                if(range != ""){
                    // std::cout << "range = " << range << "\n";
                    range = range.replace(range.find("bytes="), 6, "");
                    std::vector<std::string> range_vec = other::split(range, "-");
                    long long int start = std::stoi(range_vec[0], nullptr);
                    // std::cout << "here12345!!!\n";
                    // std::cout << "vec0 = "<< range_vec[0]<< " vec2 = "<< range_vec[1]<< " boolean = "<< (range_vec[1] == "") << "\n";
                    long long int end = fileSize;
                    if(range_vec.size() >= 2){
                        end = range_vec[1]!=""? std::stoi(range_vec[1], nullptr): fileSize-1;
                    }
                    // std::cout << "here444!!!\n";
                    long long int chunksize = (end-start)+1;
                    if (chunksize >= 300000){
                        chunksize = 300000;
                        end = chunksize-1+start;
                    }
                    // std::cout << "start = " << start << " " << "end = " << end <<"\n";
                    std::string data = other::myReadFileWithSize("./video/2.mp4",start, chunksize);
                    if(start+chunksize >= fileSize) end = fileSize-1;
                    // std::cout << "fuck bro" << data << "\n";
                    std::stringstream tmp;
                    // std::cout << "here123!!!\n";
                    tmp << "HTTP/1.1 206 Partial Content\r\n"
                        << "Content-Type: video/mp4\r\n" \
                        << "Accept-Ranges: bytes\r\n" \
                        << "Connection: keep-alive\r\n" \
                        << "Content-Length: "<<  std::to_string(chunksize) <<  "\r\n" \
                        << "Content-Range: " << "bytes "+std::to_string(start)<<"-"+std::to_string(end)<<"/"+std::to_string(fileSize)<<"\r\n" \
                        << "\r\n" 
                        << data;
                    // std::cout << "here!!!\n";
                    res = tmp.str();
                }
                else{
                    // std::string data = other::myReadFile("./video/1.mp4");
                    // std::stringstream tmp;
                    std::string data = other::myReadFileWithSize("./video/2.mp4", 0, 300000);
                    std::stringstream tmp;
                    tmp << "HTTP/1.1 200 OK\r\n"
                        << "Content-Type: video/mp4\r\n" \
                        << "Connection: keep-alive\r\n" \
                        << "Content-Length: "<< 300000 << "\r\n" \
                        << "\r\n"
                        << data;
                    res = tmp.str();
                }
            }
            else if(pathname == "/audio.html"){
                std::string html = other::myReadFile("./staticFile/audio.html");
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
            }
            else if(pathname == "/1.mp3"){
                std::string data = other::myReadFile("./audio/1.mp3");
                std::stringstream tmp;
                tmp << "HTTP/1.1 200 OK\r\n"
                    << "Content-Type: audio/mpeg\r\n" \
                    << "Connection: keep-alive\r\n" \
                    << "Content-Length: "<< data.size() << "\r\n" \
                    << "\r\n"
                    << data;
                res = tmp.str();
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
