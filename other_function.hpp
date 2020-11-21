#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream> 
#include <stdlib.h>
#include <time.h>
class HttpRequest {
    public:
        std::string raw_request;
        std::map<std::string, std::string> header;
        int badRequest = 0;
        void parseRequest(std::string source);
};
namespace other{
    std::vector<std::string> split(std::string s, std::string delimiter) {
        size_t pos_start = 0, pos_end, delim_len = delimiter.length();
        std::string token;
        std::vector<std::string> tmp;
        while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
            token = s.substr (pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            tmp.push_back(token);
        }
        if(s.substr(pos_start) != "") tmp.push_back(s.substr(pos_start));
        return tmp;
    }
    std::string myReadFile(std::string s){
        std::ifstream input_file(s, std::ios::in);
        if( !input_file ){
            std::cerr << "File could not be opened\n";
        }
        std::string content((std::istreambuf_iterator<char>(input_file)), (std::istreambuf_iterator<char>()));
        // std::cout << content;
        return content;
    }
    std::string parseBoardData(std::string s){
        std::vector<std::string> tmp = split(s, "&");
        std::vector<std::string> vec_for_name = split(tmp[0], "=");
        std::vector<std::string> vec_for_mes = split(tmp[1], " ");
        vec_for_mes = split(vec_for_mes[0], "=");
        return vec_for_name[1]+ " " +vec_for_mes[1];
    }
    std::map<std::string, std::string> getUsernameAndPassword(HttpRequest req){
        std::vector<std::string> body = other::split(req.raw_request, "\r\n\r\n");
        int i_auto = std::stoi(req.header["Content-Length"], nullptr);
        body[1] = body[1].substr(0, i_auto);
        std::vector<std::string> tmp_body = other::split(body[1], "&");
        std::string username = tmp_body[0].substr(tmp_body[0].find("=")+1);
        std::string password = tmp_body[1].substr(tmp_body[1].find("=")+1);
        std::string action = tmp_body[2].substr(tmp_body[2].find("=")+1);
        std::map<std::string, std::string> ans;
        ans["username"] = username;
        ans["password"] = password;
        ans["action"] = action;
        std::cout << "username = " << ans["username"] << "\n"; 
        std::cout << "password = " << ans["password"] << "\n";
        std::cout << "action = " << ans["action"] << "\n";
        // if (ans["password"] == "881207") std::cout << "pass fuckbro\n";
        // if (ans["action"] == "login\n") std::cout << "fuckbro\n";
        return ans;
    }
    std::string generateUUID(){
        srand(time(NULL));
        char *strUuid = new char[150];
        sprintf(strUuid, "%x%x-%x-%x-%x-%x%x%x", 
            rand(), rand(),                 // Generates a 64-bit Hex number
            rand(),                         // Generates a 32-bit Hex number
            ((rand() & 0x0fff) | 0x4000),   // Generates a 32-bit Hex number of the form 4xxx (4 indicates the UUID version)
            rand() % 0x3fff + 0x8000,       // Generates a 32-bit Hex number in the range [0x8000, 0xbfff]
            rand(), rand(), rand());        // Generates a 96-bit Hex number
        delete [] strUuid;
        std::string UUID_str(strUuid);
        return UUID_str;
    }   
}