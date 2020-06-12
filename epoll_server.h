#ifndef _EPOLL_SERVER_
#define _EPOLL_SERVER_

#include <sys/epoll.h>
#include "abstract_server.h"
#include "simple_http_handler.h"

const int DFT_EPOLL_SIZE = 120;

class EpollServer : public AbstractServer
{
public:
    EpollServer(short port = DFT_PORT, int queue = DFT_QUEUE_NUM);
    virtual ~EpollServer();
    int Accept() override;
private:
    void Working(int fd) override;
    void HandleNewConnection(int fd);
    void HandleEpollIn(int fd);
    void HandleEpollOut(int fd);
    void HandleEpollException(int fd);
    int _epollfd;
    epoll_event _events[DFT_EPOLL_SIZE];
    HttpHandler http;
};

#endif