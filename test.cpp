#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
enum METHOD { GET, POST, DELETE, PUT};

int main(){
    for(int i = GET; i != PUT; i++){
        std::cout << i << "\n";
    }    
    return 0;
}