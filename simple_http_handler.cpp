#include "simple_http_handler.h"
#include <string.h>
#include <sys/socket.h>
#include <stdio.h>

int g_httpHandleTimes = 0;

HttpHandler::HttpHandler() {};
HttpHandler::~HttpHandler() {};

int HttpHandler::HandleRequest(int fd, const char *req)
{
    Handle200(fd);
    //printf("H--send rsp to fd(%u)\n", fd);
    g_httpHandleTimes++;
    printf("Handle http times(%u).\n", g_httpHandleTimes);
    return 0;
}

void HttpHandler::Handle404(int fd)
{
    const char * first_line="HTTP/1.1 404 NOT Found\n";
    const char* blank_line="\n";
    const char* type_line="\" Content-Type:\"\" text/html;charset=utf-8\"\n";//在headler部分也加上，增强代码的健壮性，保证任何浏览器都不乱码
    const char* html="<head><meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\"> </head>"
        "<h1>404 Not Found, To be opening.</h1>";
    send(fd,first_line,strlen(first_line),0);
    send(fd,type_line,strlen(type_line),0);
    send(fd,blank_line,strlen(blank_line),0);
    send(fd,html,strlen(html),0);
}

void HttpHandler::Handle200(int fd)
{
    const char * first_line="HTTP/1.1 200 OK\n";
    const char* blank_line="\n";
    const char* type_line="\" Content-Type:\"\" text/html;charset=utf-8\"\n";//在headler部分也加上，增强代码的健壮性，保证任何浏览器都不乱码
    const char* html="<head><meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\"> </head>"
        "<h1>Nothing to do. To be opening ~_~.</h1>";
    send(fd,first_line,strlen(first_line),0);
    send(fd,type_line,strlen(type_line),0);
    send(fd,blank_line,strlen(blank_line),0);
    send(fd,html,strlen(html),0);
}