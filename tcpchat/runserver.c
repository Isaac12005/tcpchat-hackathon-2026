#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <poll.h>

#define MAX_MSG_LEN 1024

void sendToAll(int clients[], int index, char buf[]){
    
}

int main(int argc, char const* argv[]){
    ssize_t msglen;
    int inputSock, clientSock, active;
    int clients[10];
    int clientsConnected = 0;
    struct sockaddr_in address;
    socklen_t socklen = sizeof(address);
    char buf[MAX_MSG_LEN] = {0};
    struct pollfd fds[11];

    if((inputSock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket");
        return EXIT_FAILURE;
    }
    
    /* Allows reuse of addr and multiple sockets to bind to an identical socket addr*/
    int opt = 1;
    setsockopt(inputSock, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt));
    setsockopt(inputSock, SOL_SOCKET, SO_REUSEPORT , &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_port = htons(43501);
    /* Binds socket to localhost */
    if((inet_pton(address.sin_family, "127.0.0.1", &address.sin_addr)) != 1){
        perror("inet_pton");
        return EXIT_FAILURE;
    }
    /* INADDR_ANY will bind socket to any local interface. Replace inet_pton w/ below if desired. */
    /* address.sin_addr.s_addr = INADDR_ANY; */

    if((bind(inputSock, (struct sockaddr*)&address, sizeof(address))) < 0){
        perror("bind");
        return EXIT_FAILURE;
    }

    if((listen(inputSock, 5)) < 0){
        perror("listen");
        return EXIT_FAILURE;
    }

    /* fds after inputsock bound */
    fds[0].fd = inputSock;
    fds[0].events = POLLIN;
    int nfds = 1;
    while(1){
        active = poll(fds,nfds,100);
        /* handle client connections */
        if(clientsConnected < (sizeof(clients) / sizeof(clients[0]))){
            if(fds[0].revents & POLLIN){
                /* refresh socklen or accept() starts getting weird */
                socklen = sizeof(address);
                if((clients[clientsConnected] = accept(inputSock, (struct sockaddr*)&address, &socklen)) < 0){
                    perror("accept");
                    continue;
                } else {
                    fds[clientsConnected+1].fd = clients[clientsConnected];
                    fds[clientsConnected+1].events = POLLIN;
                    printf("Client %d Connected\n", clientsConnected + 1);
                    clientsConnected++;
                    nfds = clientsConnected + 1;
                }
            }
        }
        active = poll(fds,nfds,100);
        /* handle client traffic and disconnects */
        for(size_t i = 0; i < (sizeof(clients)/sizeof(clients[0])); i++){
            if(clients[i] != 0 && fds[i+1].revents & POLLIN){
                msglen = recv(clients[i], buf, sizeof(buf) - 1, 0);
                if(msglen > 0){
                    buf[msglen] = '\0';
                    printf("MSG: %s\n", buf);
                } else if (msglen == 0) { 
                    printf("Client %d Disconnected\n", i + 1);
                    close(clients[i]);
                    /* rm socket and fd(file descriptors for socket) */
                    clients[i]= 0;
                    fds[i+1].fd = -1;
                } else {
                    perror("recv");
                }

            }
        }
    }
    close(inputSock);
    return EXIT_SUCCESS;
}
