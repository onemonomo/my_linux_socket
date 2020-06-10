#ifndef _EPOLL_SERVER_
#define _EPOLL_SERVER_

const int DFT_EPOLL_SIZE = 20;
class EpollServer : public MyServer
{
public:
    int Access();
private:
    int _epollfd;
    epoll_event _events[DFT_EPOLL_SIZE];
}

#endif