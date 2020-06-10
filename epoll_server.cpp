#include "epoll_server.h"
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

// 感谢下列博客作者
// https://blog.csdn.net/u012206617/article/details/89301624
// https://blog.csdn.net/xiajun07061225/article/details/9250579

EpollServer::EpollServer(short port, int queue) : AbstractServer(port, queue) {}
EpollServer::~EpollServer() {}

int EpollServer::Accept()
{
    _epollfd = epoll_create(DFT_EPOLL_SIZE);
    epoll_event single;
    single.events = EPOLLIN | EPOLLET;
    single.data.fd = _listenfd;

    // 添加listenfd到epoll
    if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, _listenfd, &single) < 0) {
        perror("add listenfd to epoll.");
        return -1;
    }
    int currentEventNum = 1;
    while(true) {
        int num = epoll_wait(_epollfd, _events, currentEventNum, 0); // 修改时延
        if (num == -1) {
            perror("epoll wait.");
            return -2;
        }
        if (num == 0) {
            printf("no connection.\n"); // 会不会疯狂输出？
            continue;
        }
        for (int i = 0; i < num; ++i) {
            if (_events[i].data.fd == _listenfd) {
                HandleNewConnection(_listenfd);
            } else {
                if (_events[i].data & EPOLLIN) // 接收数据
                    HandleEpollIn(_events[i].data.fd);
            }
        }
        // 怎么退出
    }
    // 析构close _epollfd;
}

void EpollServer::HandleNewConnection(int fd)
{
    int connfd = accept(listenfd,(sockaddr *)&clientaddr, &clilen);
    epoll_event single;
    single.data.fd = connfd;
    single.events = EPOLLIN|EPOLLET;
    epoll_ctl(_epollfd, EPOLL_CTL_ADD, connfd, &single);
}

void EpollServer::HandleEpollIn(int fd)
{
    if (fd < 0) return;

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int len = recv(fd, buffer, sizeof(buffer), 0);
    printf("Client[fd:%u]:%s", fd, buffer);
    if (strcmp(buffer, "exit\n") == 0) {
        char end[] = "Good Bye.\n";
        send(fd, end, sizeof(end), 0);
        printf("Client request closing connection.\n");

        epoll_event single;
        single.data.fd = fd;
        single.events = EPOLLIN|EPOLLET;
        epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd,&single);
        close(fd);
        return;
    }
    char ans[] = "ToDo.\n";
    send(fd, ans, sizeof(ans), 0);
    // 需不需要重新epoll_ctl mod?
}

void EpollServer::Working(int fd) {}