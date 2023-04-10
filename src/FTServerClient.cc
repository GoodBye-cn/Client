#include "FTServerClient.h"

FTServerClient::FTServerClient() {
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_port = htons(8080);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_family = AF_INET;

    fd = socket(AF_INET, SOCK_STREAM, 0);

    int res = connect(fd, (sockaddr*)&serv_addr, sizeof(serv_addr));
    if (res < 0) {
        printf("connect error\n");
        exit(1);
    }
}

FTServerClient::~FTServerClient() {}

int FTServerClient::request_file() {
    filefd = open("/mnt/e/resource/dest/springboot-vue-demo.zip", O_WRONLY | O_CREAT);
    const char* path = "/mnt/e/resource/src/springboot-vue-demo.zip";
    req.length = strlen(path);
    memcpy(req.path, path, req.length);

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

void FTServerClient::get_response() {
    char buf[1024];
    int readBytes = 0;
    int headSize = sizeof(Response);
    bool recvData = false;
    char* data;
    Response resp;
    int ret;
    while (true) {
        if (!recvData) {
            readBytes += recv(fd, buf + readBytes, sizeof(buf) - readBytes, 0);
            if (readBytes > sizeof(Response)) {
                memcpy(&resp, buf, sizeof(Response));
                printf("len: %d", resp.length);
                data = new char[resp.length];
                // data = buf + sizeof(Response);
                memcpy(data, buf + headSize, readBytes - headSize);
                recvData = true;
                continue;
            }
        }
        ret = recv(fd, data + readBytes - headSize, resp.length + headSize - readBytes, 0);
        if (ret <= 0) {
            break;
        }
        readBytes += ret;
        // 传输完成
        if (readBytes == headSize + resp.length) {
            int writeBytes = 0;
            while (writeBytes < resp.length) {
                writeBytes += write(filefd, data + writeBytes, resp.length - writeBytes);
            }
            close(filefd);
            break;
        }
    }
    close(fd);
    exit(0);
}