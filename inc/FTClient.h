#ifndef _FTCLIENT_H
#define _FTCLIENT_H

#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PATHLEN 1024

struct Request {
    int length;
    char path[PATHLEN];
};

struct Response {
    int size;
};

class FTClient {
public:
    FTClient();
    ~FTClient();
    // 一次链接一个请求
    void run();
    // 多次连接一个请求
    void run(int num);
private:
    void set_network();
    void set_file_request(const char* file_path);
    void recv_file_write();
    void close_fd();
private:
    const char* ip = "127.0.0.1";
    const char* request_file_path = "/mnt/e/resource/src/springboot-vue-demo.zip";
    const char* write_file_path = "/mnt/e/resource/dest/demo4.zip";
    int port = 8888;
    struct sockaddr_in serv_addr;
    int sockfd, filefd;
    struct Request request;
    struct Response response;

    char* file_buff;
};

#endif