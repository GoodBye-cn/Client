#include "FTServerClient.h"

FTServerClient::FTServerClient() {
    struct Request req;
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_port = htons(8080);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_family = AF_INET;

    int fd = socket(AF_INET, SOCK_STREAM, 0);

    int res = connect(fd, (sockaddr*)&serv_addr, sizeof(serv_addr));
    if (res < 0) {
        printf("connect error\n");
        exit(1);
    }

    char buf[512] = { 0 };
    size_t ret = 0;
    ret = send(fd, (void*)&req, req.length + sizeof(int), 0);
    printf("ret: %ld", ret);
    if (ret < 0) {
        printf("send error\n");
        close(fd);
        exit(0);
    }
}

FTServerClient::~FTServerClient() {}