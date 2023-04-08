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

struct Request {
    int length;
    char path[256];
};

struct Response {
    int length;
    char end[4];
};

class FTServerClient {
public:
    FTServerClient();
    ~FTServerClient();

private:
    /* data */
};


#endif