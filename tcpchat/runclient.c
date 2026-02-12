#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX_MSG_LEN 1024

int main(int argc, char const* argv[]){
    int clientSock;
    struct sockaddr_in address;
    char buf[MAX_MSG_LEN] = {0};

    if((clientSock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket");
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(43501);
    if((inet_pton(address.sin_family, "127.0.0.1", &address.sin_addr)) != 1){
        perror("inet_pton");
        return 1;
    }
    if((connect(clientSock, (struct sockaddr*)&address, sizeof(address))) < 0){
        perror("connect");
        return 1;
    } else {
        printf("Successfully Connected to Server\n");
    }
    close(clientSock);
}
