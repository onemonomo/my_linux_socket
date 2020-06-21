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
#include <iostream>


AbstractServer::AbstractServer(short port, int queue) : port_(port), listenQueueNum_(queue) {}
AbstractServer::~AbstractServer()
{
    if (listenfd_ != -1) {
        printf("close listen fd[%u]\n", listenfd_);
        close(listenfd_);
    }
}

int AbstractServer::Init()
{
    printf("Start to creat socket and listen.\n");
    listenfd_ = socket(AF_INET, SOCK_STREAM, 0);
    printf("creaat listen fd[%u]\n", listenfd_);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port_);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); //https://blog.csdn.net/jeffasd/article/details/51461568
    
    if (bind(listenfd_, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) == -1) {
        perror("bind");
        return -1;
    }

    if (listen(listenfd_, listenQueueNum_) == -1) {
        perror("listen");
        return -1;
    }

    return 0;
}

void AbstractServer::SetPort(int port) {
    port_ = port;
}
