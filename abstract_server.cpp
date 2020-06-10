#include "abstract_server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>


AbstractServer::AbstractServer(short port, int queue) : _port(port), _queueNum(queue) {}
AbstractServer::~AbstractServer()
{
    if (_listenfd != -1) {
        printf("close listen fd[%u]\n", _listenfd);
        close(_listenfd);
    }
}

int AbstractServer::Init()
{
    printf("Start to creat socket and listen.\n");
    _listenfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("creaat listen fd[%u]\n", _listenfd);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(_port);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); //https://blog.csdn.net/jeffasd/article/details/51461568
    
    if (bind(_listenfd, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) == -1) {
        perror("bind");
        return -1;
    }

    if (listen(_listenfd, _queueNum) == -1) {
        perror("listen");
        return -1;
    }

    return 0;
}

void AbstractServer::SetPort(int port) {
    _port = port;
}
