#include "my_server.h"
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
#include <thread>
#include <iostream>


MyServer::MyServer(short port, int queue) : _port(port), _queueNum(queue) {}
MyServer::~MyServer()
{
    if (_listenfd != -1) {
        printf("close listen fd[%u]\n", _listenfd);
        close(_listenfd);
    }
}

int MyServer::Init()
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

void MyServer::Working(int fd)
{
    if (fd < 0) return;

    char buffer[1024];
    while(true) {
        memset(buffer, 0, sizeof(buffer));
        int len = recv(fd, buffer, sizeof(buffer), 0);
        printf("Client:%s", buffer);
        if (strcmp(buffer, "exit\n") == 0) {
            char end[] = "Good Bye.\n";
            send(fd, end, sizeof(end), 0);
            printf("Client request closing connection.\n");
            return;
        }
        char ans[] = "ToDo.\n";
        send(fd, ans, sizeof(ans), 0);
    }
}

int MyServer::Accept()
{
    sockaddr_in clientAddr;
    socklen_t length = sizeof(clientAddr);
    //int connectdfd = accept(_listenfd, (sockaddr*)&clientAddr, &length);
    int connectdfd = accept(_listenfd, nullptr, nullptr);
    if (connectdfd < 0) {
        perror("accept");
        return -1;
    }

    Working(connectdfd);
    close(connectdfd);
    return 0;
}