#ifndef _EPOLL_SERVER_
#define _EPOLL_SERVER_

#include "abstract_server.h"

const int DFT_EPOLL_SIZE = 20;
class EpollServer : public MyServer
{
public:
    EpollServer();
    ~virtual EpollServer();
    int Accept() override;
private:
    void Working(int fd) override;
    int _epollfd;
    epoll_event _events[DFT_EPOLL_SIZE];
}

#endif