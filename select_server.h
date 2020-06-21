#ifndef _SELECT_SERVER_H_
#define _SELECT_SERVER_H_

#include "abstract_server.h"
#include "simple_http_handler.h"
#include <sys/select.h>


class SelectServer : public AbstractServer
{
public:
    SelectServer(short port = DFT_PORT, int queue = DFT_QUEUE_NUM);
    virtual ~SelectServer();
    int Start() override;
private:
    void HandleNewConnection(int fd) override;
    void HandleClientIn(int fd) override;
    fd_set selfSet_;
    fd_set selectSet_; // use by Select
    int maxfd_;
    int clientList_[FD_SETSIZE]; // TODO : change to list
    int clientListRange_ = 0;
    HttpHandler http_;
};

#endif