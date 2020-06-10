#include <stdlib.h>
#include "my_server.h"
#include "epoll_server.h"

int main1(int argc, char *argv[])
{
    MyServer tmp;
    if (argc == 2) {
        tmp.SetPort(atoi(argv[1]));
    }
    tmp.Init();
    tmp.Accept();
    return 0;
}

int main(int argc, char *argv[])
{
    EpollServer tmp;
    if (argc == 2) {
        tmp.SetPort(atoi(argv[1]));
    }
    tmp.Init();
    tmp.Accept();
    return 0;
}