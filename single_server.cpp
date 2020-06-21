#include "single_server.h"
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


SingleServer::SingleServer(short port, int queue) : AbstractServer(port, queue) {}
SingleServer::~SingleServer() {}

void SingleServer::HandleNewConnection(int fd)
{
    sockaddr_in clientAddr;
    socklen_t length = sizeof(clientAddr);
    //int connectdfd = accept(listenfd_, (sockaddr*)&clientAddr, &length);
    connectdfd_ = accept(fd, nullptr, nullptr);
    if (connectdfd_ < 0) {
        perror("accept");
        exit(0);
    }
}

void SingleServer::HandleClientIn(int fd)
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

int SingleServer::Start()
{

    HandleNewConnection(listenfd_);
    HandleClientIn(connectdfd_);
    close(connectdfd_);
    return 0;
}