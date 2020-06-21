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
        clientList_[i] = -1;
    }
}

SelectServer::~SelectServer()
{

}

int SelectServer::Start()
{
    FD_ZERO(&selfSet_);
    // FD_ZERO(selectSet_);
    FD_SET(listenfd_, &selfSet_);
    SetNonBlocking(listenfd_);
    maxfd_ = listenfd_;
    while (true) {
        printf("true.\n");
        selectSet_ = selfSet_;
        int readyNum = select(maxfd_ + 1, &selectSet_, nullptr, nullptr, nullptr); // block
        if (FD_ISSET(listenfd_, &selectSet_)) {
            HandleNewConnection(listenfd_);
            if (--readyNum <= 0) continue;
        }

        int newClientfd = -1;
        for (int i = 0; i <= clientListRange_; ++i) {
            if ((newClientfd = clientList_[i]) < 0) continue;
            if (FD_ISSET(newClientfd, &selectSet_)) {
                HandleClientIn(newClientfd);
                clientList_[i] = -1;
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
    while ((connfd = accept(listenfd_, nullptr, nullptr)) > 0) {
        int i;
        for (i = 0; i < FD_SETSIZE; ++i) {
            if (clientList_[i] < 0) {
                clientList_[i] = connfd;
                break;
            }
        }
        if (i == FD_SETSIZE) {
            perror("max num of select size");
            return;
        }
        if (i > clientListRange_) clientListRange_ = i;
        if (connfd > maxfd_) maxfd_ = connfd;
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

    http_.HandleRequest(fd, buffer);
    close(fd);
    FD_CLR(fd, &selfSet_);
}
