#ifndef _EPOLL_SERVER_
#define _EPOLL_SERVER_

#include "abstract_server.h"

const int DFT_EPOLL_SIZE = 20;
class EpollServer : public AbstractServer
{
public:
    EpollServer(short port = DFT_PORT, int queue = DFT_QUEUE_NUM);
    ~virtual EpollServer();
    int Accept() override;
private:
    void Working(int fd) override;
    void HandleNewConnection(int fd);
    void HandleEpollIn(int fd);
    void HandleEpollOut(int fd);
    int _epollfd;
    epoll_event _events[DFT_EPOLL_SIZE];
}

#endif