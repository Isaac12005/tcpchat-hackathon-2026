#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, const char* argv[]){
    int newsock;
    struct sockaddr_in address;
    socklen_t socklen = sizeof(address);

    if((newsock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket");
        return 1;
    }
    
    /* Allows reuse of addr and multiple sockets to bind to an identical socket addr*/
    int opt = 1;
    setsockopt(newsock, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt));
    setsockopt(newsock, SOL_SOCKET, SO_REUSEPORT , &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_port = htons(43501);
    /* Binds socket to localhost */
    if((inet_pton(address.sin_family, "127.0.0.1", &address.sin_addr)) != 1){
        perror("inet_pton");
        return 1;
    }
    /* INADDR_ANY will bind socket to any local interface. Replace inet_pton if desired. */
    /* address.sin_addr.s_addr = INADDR_ANY; */

    if((bind(newsock, (struct sockaddr*)&address, sizeof(address))) < 0){
        perror("bind");
        return 1;
    }

    close(newsock);
    return 0;
}


