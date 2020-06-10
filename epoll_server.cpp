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

// https://blog.csdn.net/u012206617/article/details/89301624
int EpollServer::Access()
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
        switch(num) {
            case(-1) :
                perror("epoll wait.");
                return -2;
            case(0) :
                printf("no connection.\n"); // 会不会疯狂输出？
                break;
            default :
                for (int i = 0; i < num; ++i) {
                    if (_events[i].data.fd == _listenfd) {
                        continue; // TODO
                    } else {
                        Working();
                    }
                }
        }
        // 怎么退出
    }
    // 析构close _epollfd;
}