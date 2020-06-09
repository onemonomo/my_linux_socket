#ifndef _MY_SERVER_
#define _MY_SERVER_

const int DFT_PORT = 7000;
const int DFT_QUEUE_NUM = 20;

class MyServer
{
public:
	MyServer(short port = DFT_PORT, int queue = DFT_QUEUE_NUM);
	virtual ~MyServer();
	int Init();
	int Accept();
    void SetPort(int port) {
        _port = port;
    }
private:
	virtual void Working(int fd);
private:
	short _port;
	int _queueNum;
	int _listenfd = -1;
};

#endif