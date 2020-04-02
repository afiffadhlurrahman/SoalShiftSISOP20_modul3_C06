#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define MAXCHAR 1000
#define PORT 8080

int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    
    FILE *file; 
    char *fname;
    fname = "akun.txt";
    int *flag = 0;

    char str[MAXCHAR];
    char *buffer1[MAXCHAR];
    
    // char *hello = "Hello from server";
      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    valread = read( new_socket , buffer, 1024);
    if(strcmp(buffer,"login")==0){
        file = fopen(fname, "w+");
        valread = read( new_socket , buffer, 1024);
        while (fgets(str, MAXCHAR, file) != NULL){
            if(strcmp(str,buffer) == 0){
                printf("Auth success\n");
                *flag = 1;
            }
        }
        if(flag == 0){
            printf("Auth Failed\n");
        }
        send(new_socket , flag , sizeof(flag) , 0 );
    }
    else if(strcmp(buffer,"register")==0){
        printf("regis\n");
    }
    
    
    
    return 0;
}
