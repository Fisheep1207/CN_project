#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <fstream>
#include <sstream> 

int main(){
    std::ofstream boardFile;
    boardFile.open ("board_content.txt", std::ios::app);
    // boardFile << (header["nAmE"] + " " +header["mEsSaGe"]) << "\n";
    boardFile << "abcc" << "\r\n";
    boardFile.close();
    return 0;
}