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
#include <stdlib.h>
#include <time.h>
// std::string generateUUID(){
//     srand(time(NULL));
//     char *strUuid = new char[1500];
//     sprintf(strUuid, "%x%x-%x-%x-%x-%x%x%x", 
//         rand(), rand(),                 // Generates a 64-bit Hex number
//         rand(),                         // Generates a 32-bit Hex number
//         ((rand() & 0x0fff) | 0x4000),   // Generates a 32-bit Hex number of the form 4xxx (4 indicates the UUID version)
//         rand() % 0x3fff + 0x8000,       // Generates a 32-bit Hex number in the range [0x8000, 0xbfff]
//         rand(), rand(), rand());        // Generates a 96-bit Hex number
//     delete [] strUuid;
//     std::string UUID_str(strUuid);
//     return UUID_str;
// }
int main(){
    std::ifstream f("404.html");
    std::cout << f.good() << "\n";
    // generateUUID();
    // std::ofstream boardFile;
    // boardFile.open ("board_content.txt", std::ios::app);
    // // boardFile << (header["nAmE"] + " " +header["mEsSaGe"]) << "\n";
    // boardFile << "abcc" << "\r\n";
    // boardFile.close();
    return 0;
}