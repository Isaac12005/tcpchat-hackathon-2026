#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX_MSG_LEN 1024

int main(int argc, char const* argv[]){
    ssize_t msglen;
    int inputSock, clientSock;
    struct sockaddr_in address;
    socklen_t socklen = sizeof(address);
    char buf[MAX_MSG_LEN] = {0};

    if((inputSock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket");
        return 1;
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
        return 1;
    }
    /* INADDR_ANY will bind socket to any local interface. Replace inet_pton w/ below if desired. */
    /* address.sin_addr.s_addr = INADDR_ANY; */

    if((bind(inputSock, (struct sockaddr*)&address, sizeof(address))) < 0){
        perror("bind");
        return 1;
    }

    if((listen(inputSock, 5)) < 0){
        perror("listen");
        return 1;
    }

    if((clientSock = accept(inputSock, (struct sockaddr*)&address, &socklen)) < 0){
        perror("accept");
        return 1;
    } else {
        printf("Client Connected\n");
    }
    /*
    msglen = read(inputSock, buf, MAX_MSG_LEN);
    printf("%s\n", buf);
    */
    close(inputSock);
    close(clientSock);

    return 0;
}

