#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <poll.h>
#include <string.h>

#define MAX_MSG_LEN 1024

void sendToAll(int clients[], int clientsConnected, int index, char buf[]){
    char *t = strtok(buf, ":");
    t = strtok(NULL, ":");
    for(size_t i = 0; i < clientsConnected; i++){
        if(clients[i] != 0 && i != index){
            send(clients[i], t, strlen(t), 0);
        }
    }
}

int parseBuffer(int clients[], int index, char buf[], char names[][30]){
    printf("Entering Parsing Logic\n");
    size_t i = 0;
    char *loc = strcasestr(buf, "name:");
    if(loc != NULL){
        loc += 5;
        while(*loc != '\0' && i < 29){
            names[index][i++] = *loc++;
        }
        names[index][i] = '\0';
        return 1;
    }
    loc = strcasestr(buf, "msg:");
    if(loc != NULL){
        return 2;
    }
    loc = strcasestr(buf, "pmsg@");
    if(loc != NULL){
        return 3;
    }
    loc = strcasestr(buf, "list:");
    if(loc != NULL){
        return 4;
    }
    loc = strcasestr(buf, "ai:");
    if(loc != NULL){
        return 5;
    } else {
        return 0;
    }

}

int main(int argc, char const* argv[]){
    ssize_t msgLen;
    int inputSock, clientSock, active;
    int clients[100];
    int clientsConnected = 0;
    int parsed;
    struct sockaddr_in address;
    socklen_t socklen = sizeof(address);
    char buf[MAX_MSG_LEN] = {0};
    char names[100][30] = {0};
    char* msgError = "An Error Occurred, please try again.";
    struct pollfd fds[101];

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
    /* INADDR_ANY will bind socket to any local interface. Replace inet_pton if statement w/ below if desired. */
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
        /* need to hand reoccupying disconnected connections */
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
        /* need to handle disconnected client sockets able to be reoccupied */
        for(size_t i = 0; i < (sizeof(clients)/sizeof(clients[0])); i++){
            if(clients[i] != 0 && fds[i+1].revents & POLLIN){
                msgLen = recv(clients[i], buf, sizeof(buf) - 1, 0);
                if(msgLen > 0){
                    int parsed = parseBuffer(clients, i, buf, names);
                    switch (parsed){
                        case 1: 
                            printf("Client Name Updated\n");
                            break;
                        case 2: 
                            sendToAll(clients, clientsConnected, i, buf);
                            break;
                            /*
                        case 3:
                            privateMessage();
                            break;
                        case 4: 
                            listAllClients();
                            break;
                        case 5:
                            promptAI();
                            break;
                            */
                        default:
                            send(clients[i], msgError, strlen(msgError),0);

                    }
                } else if (msgLen == 0) { 
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
