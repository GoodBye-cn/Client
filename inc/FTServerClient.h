#ifndef FTPSERVERCLIENT_H
#define FTPSERVERCLIENT_H

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
    char end[4];
};

class FTServerClient {
public:
    FTServerClient();
    ~FTServerClient();
    int request_file();
    void get_response();
private:
    int fd;
    int filefd;
    struct Request req;
    struct Response resp;
};


#endif