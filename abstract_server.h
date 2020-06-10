#ifndef _ABSTRACT_SERVER_H_
#define _ABSTRACT_SERVER_H_


const int DFT_PORT = 7000;
const int DFT_QUEUE_NUM = 20;

class AbstractServer
{
public:
	AbstractServer(short port = DFT_PORT, int queue = DFT_QUEUE_NUM);
	virtual ~AbstractServer();
	int Init();
	virtual int Accept() = 0;
    void SetPort(int port);
private:
	virtual void Working(int fd) = 0;
protected:
	short _port;
	int _queueNum;
	int _listenfd = -1;
}

#endif