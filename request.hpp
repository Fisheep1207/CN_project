#include "other_function.hpp"
class HttpRequest {
    public:
        std::string raw_request;
        std::map<std::string, std::string> header;
        int badRequest = 0;
        void parseRequest(std::string source);
};

void HttpRequest::parseRequest(std::string source){
    raw_request = source;
    std::vector<std::string> tmp_req = other::split(source, "\r\n");
    std::vector<std::string> first_line_vec = other::split(tmp_req[0], " ");
    // std::cout << "WTF\n";
    if(first_line_vec[0] == "GET"){    
        // std::cout << "WTF1\n";
        header["method"] = first_line_vec[0];
        // std::cout << "WTF2\n";
        header["pathname"] = first_line_vec[1];
        // std::cout << "WTF3\n";
        header["httpVersion"] = first_line_vec[2];
        // std::cout << "WTF4\n";
        for(auto iter=tmp_req.cbegin()+1; iter != tmp_req.cend(); iter++){
            // std::cout << "*iter = " << *iter << "\n";
            std::vector<std::string> key_value = other::split(*iter, ":");
            if(key_value.size() >= 2){
                header[key_value[0]] = key_value[1];
            }
        }
        // std::cout << "WTF5\n";
    }
    else if(first_line_vec[0] == "POST"){
        // std::cout << "WTF1\n";
        header["method"] = first_line_vec[0];
        header["pathname"] = first_line_vec[1];
        header["httpVersion"] = first_line_vec[2];
        for(auto iter=tmp_req.cbegin()+1; iter != tmp_req.cend(); iter++){
            // std::cout << "*iter = " << *iter << "\n";
            std::vector<std::string> key_value = other::split(*iter, ":");
            if(key_value.size() >= 2){
                header[key_value[0]] = key_value[1];
            }
            else{
                break;
            }
        }
        // if(header["pathname"] == "/mes_board.html"){
        //     std::vector<std::string> body = other::split(source, "\r\n\r\n");
        //     int i_auto = std::stoi (header["Content-Length"], nullptr);
        //     std::ofstream boardFile;
        //     boardFile.open("board_content.txt", std::ios::app);
        //     boardFile << "<li> " << body[1].substr(0, i_auto) << " </li>\n";
        //     boardFile.close();
        // }
        // else if(header["pathname"] == "/login.html"){
        //     if(header.find("Cookie") == header.end()){
        //         std::cout << "New-Cookie" << "\n";
        //         other::generateUUID();
        //     }
        // }
    }
    else{
        badRequest = 1;
    }
    return;
};

