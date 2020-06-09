#include <stdlib.h>
#include "my_server.h"

int main(int argc, char *argv[])
{
    MyServer tmp;
    if (argc == 2) {
        tmp.SetPort(atoi(argv[1]));
    }
    tmp.Init();
    tmp.Accept();
    return 0;
}