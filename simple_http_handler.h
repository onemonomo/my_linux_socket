#ifndef _SIMPLE_HTTP_HANDLER_H_
#define _SIMPLE_HTTP_HANDLER_H_

class HttpHandler
{
public:
    HttpHandler();
    ~HttpHandler();
    int HandleRequest(int fd, const char *req);
private:
    void Handle404(int fd);
    void Handle200(int fd);
};

#endif