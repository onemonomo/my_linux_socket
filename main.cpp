#include <stdlib.h>
#include "my_server.h"
#include "epoll_server.h"
#include "select_server.h"

int main(int argc, char *argv[])
{
    AbstractServer *server;
    int port = 7000;
    if (argc >= 2) {
        if (atoi(argv[1]) == 1)
            server = new MyServer;
        if (atoi(argv[1]) == 2)
            server = new EpollServer;
        if (atoi(argv[1]) == 3)
            server = new SelectServer;
    }
    if (argc == 3) {
        port = atoi(argv[2]);
    }
    server->SetPort(port);
    server->Init();
    server->Accept();
    return 0;
}