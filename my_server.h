#ifndef _MY_SERVER_H_
#define _MY_SERVER_H_

#include "abstract_server.h"



class MyServer : public AbstractServer
{
public:
	MyServer(short port = DFT_PORT, int queue = DFT_QUEUE_NUM);
	virtual ~MyServer();
};

#endif