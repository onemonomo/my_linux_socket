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
    int Start() override;
private:
    void HandleNewConnection(int fd) override;
    void HandleClientIn(int fd) override;
    void HandleEpollException(int fd);
    int epollfd_;
    epoll_event events_[DFT_EPOLL_SIZE];
    HttpHandler http_;
};

#endif