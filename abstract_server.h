#ifndef _ABSTRACT_SERVER_H_
#define _ABSTRACT_SERVER_H_


const int DFT_PORT = 60000;
const int DFT_QUEUE_NUM = 20;

class AbstractServer
{
public:
	AbstractServer(short port = DFT_PORT, int queue = DFT_QUEUE_NUM);
	virtual ~AbstractServer();
	int Init();
	virtual int Start() = 0;
    void SetPort(int port);
private:
	virtual void HandleNewConnection(int fd) = 0;
	virtual void HandleClientIn(int fd) = 0;
protected:
	short port_;
	int listenQueueNum_;
	int listenfd_ = -1;
};

#endif