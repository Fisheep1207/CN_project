// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
using namespace std;

const int PORT = 8888;
int main(int argc, char const *argv[])
{
    int sock = 0; long long int valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello\r\nfrom\r\nclient\r\n\r\n";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("\n Socket creation error \n");
        return -1;
    }
    memset(&serv_addr, '0', sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0){
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        printf("\nConnection Failed \n");
        return -1;
    }

    send(sock , hello , strlen(hello) , 0 );
    cout << "\nAlready Send Hello!!\n";
    valread = read(sock , buffer, 1024);
    cout << buffer << "\n";
    return 0;
}