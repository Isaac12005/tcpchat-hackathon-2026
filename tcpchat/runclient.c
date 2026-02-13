#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <poll.h>

#define MAX_MSG_LEN 1024

int main(int argc, char const* argv[]){
    int clientSock, active;
    struct sockaddr_in address;
    char buf[MAX_MSG_LEN] = {0};
    struct pollfd fds[1];

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
        fds[0].fd = inputSock;
        fds[0].events = POLLIN;
        int nfds = 1;
        
    while(1){
        active = poll(fds, nfds, 100);
        printf("Enter Message to send to server: \n");
        if (fgets(buf, sizeof(buf), stdin) == NULL) {
                break;
        }
        /* buf[strcspn(buf, "\n")] = 0; */
        send(clientSock, buf, strlen(buf),0);
    }
    close(clientSock);
}
