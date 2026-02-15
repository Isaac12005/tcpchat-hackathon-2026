#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <poll.h>

#define MAX_MSG_LEN 1024
#define SERVER_ADDRESS "127.0.0.1"

int main(int argc, char const* argv[]){
    ssize_t msgLen;
    int clientSock, active;
    struct sockaddr_in address;
    char buf[MAX_MSG_LEN] = {0};
    struct pollfd fds[2];

    if((clientSock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket");
        return 1;
    }
    address.sin_family = AF_INET;
    address.sin_port = htons(43501);
    if((inet_pton(address.sin_family, SERVER_ADDRESS, &address.sin_addr)) != 1){
        perror("inet_pton");
        return 1;
    }
    if((connect(clientSock, (struct sockaddr*)&address, sizeof(address))) < 0){
        perror("connect");
        return 1;
    }

    fds[0].fd = clientSock;
    fds[0].events = POLLIN;
    int nfds = 2;
    fds[1].fd = 0; /*stdin*/
    fds[1].events = POLLIN;

    char message[MAX_MSG_LEN];
    printf("\nEnter Username:");
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';
    snprintf(message, sizeof(message), "name:%s", buf);
    send(clientSock, message, strlen(message),0);

    printf("\033[H\033[J\e[1;94m████████╗ ██████╗██████╗        ██████╗██╗  ██╗ █████╗ ████████╗\n╚══██╔══╝██╔════╝██╔══██╗      ██╔════╝██║  ██║██╔══██╗╚══██╔══╝\n   ██║   ██║     ██████╔╝█████╗██║     ███████║███████║   ██║   \n   ██║   ██║     ██╔═══╝ ╚════╝██║     ██╔══██║██╔══██║   ██║   \n   ██║   ╚██████╗██║           ╚██████╗██║  ██║██║  ██║   ██║   \n   ╚═╝    ╚═════╝╚═╝            ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   \e[0m\n---Use \"help:\" for a list of commands---\n");

    while(1){
        active = poll(fds, nfds, 100);
        if(active > 0){
            if(fds[1].revents & POLLIN){
                if (fgets(buf, sizeof(buf), stdin) == NULL) {
                        break;
                }
            buf[strcspn(buf, "\n")] = 0;
            send(clientSock, buf, strlen(buf),0);
            }
            if(fds[0].revents & POLLIN){
                msgLen = recv(clientSock, buf, sizeof(buf) - 1, 0);
                if(msgLen == 0){
                    printf("Server Disconnected. Closing.\n");
                    close(clientSock);
                    break;
                }
                buf[msgLen] = '\0';
                printf("\r%s\n", buf);
            }
        } else if(active < 0){
            perror("poll");
            close(clientSock);
            break;
        }
    }
    return 0;
}
