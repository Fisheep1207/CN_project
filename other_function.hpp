#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream> 

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
        tmp.push_back(s.substr(pos_start));
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
}