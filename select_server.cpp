#include "select_server.h"
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "socket_lib.h"

SelectServer::SelectServer(short port, int queue) : AbstractServer(port, queue)
{
    for (int i = 0; i < FD_SETSIZE; ++i) {
        client[i] = -1;
    }
}

SelectServer::~SelectServer()
{

}

int SelectServer::Accept()
{
    FD_ZERO(&selfSet_);
    // FD_ZERO(selectSet_);
    FD_SET(_listenfd, &selfSet_);
    SetNonBlocking(_listenfd);
    maxfd = _listenfd;
    while (true) {
        printf("true.\n");
        selectSet_ = selfSet_;
        int readyNum = select(maxfd + 1, &selectSet_, nullptr, nullptr, nullptr); // block
        if (FD_ISSET(_listenfd, &selectSet_)) {
            HandleNewConnection(_listenfd);
            if (--readyNum <= 0) continue;
        }

        int newClientfd = -1;
        for (int i = 0; i <= clientNum; ++i) {
            if ((newClientfd = client[i]) < 0) continue;
            if (FD_ISSET(newClientfd, &selectSet_)) {
                HandleClientIn(newClientfd);
                client[i] = -1;
                if (--readyNum <= 0) break;
            }
        }
    }
    return 0;
}

void SelectServer::HandleNewConnection(int fd)
{
    printf("SelectServer::HandleNewConnection.\n");
    int connfd = -1;
    // TODO : check while accept is ok?
    while ((connfd = accept(_listenfd, nullptr, nullptr)) > 0) {
        int i;
        for (i = 0; i < FD_SETSIZE; ++i) {
            if (client[i] < 0) {
                client[i] = connfd;
                break;
            }
        }
        if (i == FD_SETSIZE) {
            perror("max num of select size");
            return;
        }
        if (i > clientNum) clientNum = i;
        if (connfd > maxfd) maxfd = connfd;
        printf("fd set succ(%u)\n", fd);
        FD_SET(connfd, &selfSet_);
    }
}

void SelectServer::HandleClientIn(int fd)
{
    printf("SelectServer::HandleClientIn.\n");
    if (fd < 0) return;
    char buffer[10000];
    memset(buffer, 0, sizeof(buffer));
    int len = recv(fd, buffer, sizeof(buffer), 0); // TODO: use MSG_WAITALL and while
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
    http_.HandleRequest(fd, buffer);
    close(fd);
    FD_CLR(fd, &selfSet_);
}

void SelectServer::Working(int fd) {}