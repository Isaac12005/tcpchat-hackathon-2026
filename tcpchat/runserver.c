#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/select.h>

#define MAX_MSG_LEN 1024

int main(int argc, char const* argv[]){
    ssize_t msglen;
    int inputSock, clientSock;
    int clients[10];
    int clientsConnected = 0;
    struct sockaddr_in address;
    socklen_t socklen = sizeof(address);
    char buf[MAX_MSG_LEN] = {0};
    fd_set readfs;

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


    while(1){
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        active = poll(inputSock +1, )
        if((clients[clientsConnected] = accept(inputSock, (struct sockaddr*)&address, &socklen)) < 0){
            perror("accept");
            continue;
        } else {
            printf("Client #%d Connected\n", clientsConnected + 1);
            clientsConnected++;
        }
    }
    /*
    if((listen(inputSock, 5)) < 0){
        perror("listen");
        return EXIT_FAILURE;
    }
    if((clientSock = accept(inputSock, (struct sockaddr*)&address, &socklen)) < 0){
        perror("accept");
        return EXIT_FAILURE;
    } else {
        printf("Client Connected\n");
    }
    */
    /*
    msglen = read(inputSock, buf, MAX_MSG_LEN);
    printf("%s\n", buf);
    */
    close(inputSock);
    close(clientSock);

    return EXIT_SUCCESS;
}
