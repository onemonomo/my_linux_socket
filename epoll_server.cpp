#include "epoll_server.h"
#include <sys/socket.h>
#include <sys/types.h>
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


int g_established = 0;

EpollServer::EpollServer(short port, int queue) : AbstractServer(port, queue)
{
    _epollfd = -1;
}

EpollServer::~EpollServer()
{
    if (_epollfd != -1) {
        close(_epollfd);
    }
}

int EpollServer::Accept()
{
    _epollfd = epoll_create(DFT_EPOLL_SIZE);
    epoll_event single;
    single.events = EPOLLIN ;//| EPOLLET; ET
    single.data.fd = _listenfd;

    // 添加listenfd到epoll
    if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, _listenfd, &single) < 0) {
        perror("add listenfd to epoll.");
        return -1;
    }

    int currentEventNum = 1; // 这个参数没用？明显代码有问题，但是运行2客户端正常
    while(true) {
        int num = epoll_wait(_epollfd, _events, currentEventNum, 500); // 修改时延
        if (num == -1) {
            perror("epoll wait.");
            return -2;
        }
        if (num == 0) {
            // //printf("no connection.\n"); // 会不会疯狂输出？->确实会
            continue;
        }
        for (int i = 0; i < num; ++i) {
            if (_events[i].data.fd == _listenfd) {
                HandleNewConnection(_listenfd);
            } else {
                if ((_events[i].events & EPOLLHUP) || (_events[i].events & EPOLLERR) && !(_events[i].events & EPOLLIN)) // client closed
                    HandleEpollException(_events[i].data.fd);
                HandleEpollIn(_events[i].data.fd);
            }
        }
        // 怎么退出
    }
    // 析构close _epollfd;
}

void EpollServer::HandleNewConnection(int fd)
{
    printf("g_established(%u)\n", ++g_established);
    int connfd = accept(_listenfd, nullptr, nullptr);
    if (connfd < 0) {
        perror("accept");
        return;
    }
    epoll_event single;
    single.data.fd = connfd;
    single.events = EPOLLIN | EPOLLET;
    epoll_ctl(_epollfd, EPOLL_CTL_ADD, connfd, &single);
}

void EpollServer::HandleEpollIn(int fd)
{
    if (fd < 0) return;
    char buffer[10000];
    memset(buffer, 0, sizeof(buffer));
    int len = recv(fd, buffer, sizeof(buffer), 0);
    /*
    if (len == 0) {
        epoll_event single;
        single.data.fd = fd;
        // single.events = EPOLLIN | EPOLLET;
        epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd,&single);
        //printf("D--fd(%u)\n", fd);
        close(fd);
        return;
    }
    */
    http.HandleRequest(fd, buffer);
    epoll_event single;
    single.data.fd = fd;
    epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd,&single);
    close(fd);
    //char ans[] = "ToDo.\n";
    //send(fd, ans, sizeof(ans), MSG_NOSIGNAL); // 踩坑，send时，如果client已经关闭，会关闭当前进程
    // 需不需要重新epoll_ctl mod?
}

void EpollServer::HandleEpollException(int fd)
{
    epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, nullptr);
    close(fd);
}

void EpollServer::Working(int fd) {}