#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "request.hpp"
#include "response.hpp"
// const int PORT = 5000;
const unsigned int MAX_BUF_LENGTH = 10000;
std::vector<int> ccpid;
int main(int argc , char *argv[]){
    int socket_fd, new_socket_fd;
    int PORT = std::stoi(argv[1], nullptr);
    struct sockaddr_in address;
    memset((char*) &address, 0, sizeof(address));
    int addrlen = sizeof(address);
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("cannot create socket"); 
        return -1; 
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);

    if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0) { 
        perror("bind failed"); 
        return -1; 
    }
    if (listen(socket_fd, 50) < 0) { 
        perror("In listen"); 
        exit(EXIT_FAILURE); 
    }
    std::vector<char> buffer(MAX_BUF_LENGTH);
    while(1){
        // std::cout << "HI\n";
        // std::cout << other::generateUUID() << "\n";
        std::cout << "\nWait for connection!\n";
        if ((new_socket_fd = accept(socket_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
            perror("In accept");            
            exit(EXIT_FAILURE);        
        }
        int pid = fork();
        if (pid == -1){
            perror("fork error");            
            exit(EXIT_FAILURE);
        }
        else if (pid == 0){
            close(socket_fd);
            std::string rcv_data;
            int valread = read(new_socket_fd, &buffer[0], MAX_BUF_LENGTH);
            rcv_data.append(buffer.cbegin(), buffer.cend());
	    std::cout << "here = " << rcv_data << "\n";
	    HttpRequest req;
            // std::cout << "fucksis\n";
            req.parseRequest(rcv_data);
            // std::cout << "fuckbro\n";
            if (req.badRequest == 1){
                std::cout << "It's not a http request\n";
                buffer.clear();
                buffer.resize(MAX_BUF_LENGTH);
                close(new_socket_fd);
                continue;
            }
            HttpResponse res(req);
            int byte = write(new_socket_fd, res.res.c_str(), res.res.size());
            close(new_socket_fd);
            buffer.clear();
            buffer.resize(MAX_BUF_LENGTH);
            std::cout << "end response" << "\n";
            exit(0);
        }
        else{
            ccpid.push_back(pid);
            close(new_socket_fd);
            while(!ccpid.empty()){
                int status;
                for(int i = 0 ; i < ccpid.size(); i++){
                    int tmp = waitpid(ccpid[i],&status,WNOHANG);
                    if(tmp == ccpid[i]){
                        ccpid.erase(ccpid.begin()+i);
                    }
                }
            }
        }
    }
    return 0;
}
