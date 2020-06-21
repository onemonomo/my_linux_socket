# my_linux_socket

* realize 3 kinds of socket server.
1. origin linux socket()->bind()->listen()->accept()->recv()/send()
2. based on 1. add NIO skill of select() way.
3. based on 1. add NIO skill of epoll() way.

* beside socket, here is a simple http function. It can response 200 OK in any way of http requests.

![avatar](https://github.com/onemonomo/my_linux_socket/blob/master/uml.png)
