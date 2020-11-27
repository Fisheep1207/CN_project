#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream> 
#include <stdlib.h>
#include <time.h>
long long int readFileSzie(std::string filename){
    std::ifstream fin(filename, std::ifstream::in | std::ifstream::binary);
    fin.seekg(0, std::ios::end);
    long long length = fin.tellg();
    fin.close();
    return length;
}

int main(){
    long long fileSize = readFileSzie("./staticFile");
    std::cout << fileSize << "\n";

    return 0;
}