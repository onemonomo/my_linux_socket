#ifndef _SINGLE_SERVER_H_
#define _SINGLE_SERVER_H_

#include "abstract_server.h"



class SingleServer : public AbstractServer
{
public:
	SingleServer(short port = DFT_PORT, int queue = DFT_QUEUE_NUM);
	virtual ~SingleServer();
    int Start() override;
private:
    void HandleNewConnection(int fd) override;
    void HandleClientIn(int fd) override;
    int connectdfd_;
};

#endif