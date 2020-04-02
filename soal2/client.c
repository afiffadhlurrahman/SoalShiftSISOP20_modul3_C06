#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
#define MAXCHAR 1000 

int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    FILE *file; 
    char *fname;
    int flag,*reg = 0;
    int cek = 0;
    fname = "akun.txt";
    char user[10],pass[10],choice[20];
    char str[MAXCHAR],*hello;
    char cekUser[20],cekPass[20],buffer1[MAXCHAR];
    
    printf("1. login\n");
    printf("2. register\n");
    printf("choice:");
    scanf("%s",choice);
    send(sock , choice , strlen(choice) , 0 );

    if(strcmp(choice,"login")==0){
        cek = 2;
    }
    else if(strcmp(choice,"register")==0){
        cek = 1;
    }
    
    if(cek==2){
        printf("username: ");
        scanf("%s",cekUser);
        printf("password: ");
        scanf("%s",cekPass);
        sprintf(hello, "username: %s password: %s\n", cekUser, cekPass);
        send(sock , hello , strlen(hello) , 0 );
        valread = read( sock , buffer, 1024);
        
        if(flag == 1){
            printf("login success\n");
        }
        
        if(flag == 0){
            printf("login failed\n");
        }
    }
    else if(cek == 1){
        file = fopen(fname, "w");
        
        printf("username: ");
        scanf("%s",user);
        printf("password: ");
        scanf("%s",pass);

        fprintf(file,"username: %s ",user);
        fprintf(file,"password: %s\n",pass);
        printf("register success");
        *reg = 1;
        send(sock , reg , sizeof(reg) , 0 );
    }
    
    fclose(file);
    return 0;
}
