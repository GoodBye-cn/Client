#include "FTClient.h"
#include <string>


FTClient::FTClient() {}

FTClient::~FTClient() {}

void FTClient::run() {
    // 初始化网络socket
    file_buff = NULL;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_port = htons(8888);
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_family = AF_INET;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    int res = connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
    if (res < 0) {
        printf("connect error\n");
        exit(1);
    }

    // 设置文件部分
    remove(write_file_path);
    filefd = open(write_file_path, O_WRONLY | O_CREAT);
    if (filefd == -1) {
        printf("open file error\n");
        exit(1);
    }
    request.length = strlen(request_file_path);
    memcpy(request.path, request_file_path, request.length);

    char buf[512] = { 0 };
    size_t bytes = 0;
    bytes = send(sockfd, (void*)&request, request.length + sizeof(int), 0);
    printf("ret: %ld\n", bytes);
    if (bytes < 0) {
        printf("send error\n");
        close(sockfd);
        exit(0);
    }

    // 接收文件部分
    ssize_t read_bytes = 0;
    bytes = 0;
    bool head = true;
    while (true) {
        if (head) {
            bytes = read(sockfd, &response + read_bytes, sizeof(int) - read_bytes);
            if (bytes == 0) {
                printf("server disconnect\n");
                break;
            }

            if (bytes < 0) {
                printf("read head error\n");
                break;
            }
            read_bytes += bytes;
            if (read_bytes == sizeof(int)) {
                if (response.size <= 0) {
                    printf("file path error\n");
                    break;
                }
                file_buff = new char[response.size];
                read_bytes = 0;
                head = false;
            }
            continue;
        }
        bytes = read(sockfd, file_buff + read_bytes, response.size - read_bytes);
        if (bytes == 0) {
            printf("server disconnect\n");
            break;
        }
        if (bytes < 0) {
            printf("read file error\n");
            break;
        }
        read_bytes += bytes;
        if (read_bytes == response.size) {
            break;
        }
    }

    if (file_buff != NULL) {
        ssize_t write_bytes = 0;
        while (write_bytes < response.size) {
            bytes = write(filefd, file_buff + write_bytes, response.size - write_bytes);
            if (bytes < 0) {
                printf("write file error\n");
                break;
            }
            write_bytes += bytes;
        }
    }
    delete[] file_buff;
    close(filefd);
    close(sockfd);
    if (file_buff == NULL) {
        remove(write_file_path);
    }
    file_buff = NULL;
}

void FTClient::run(int num) {
    set_network();
    std::string dir_path = "/mnt/e/resource/dest/";
    std::string file_name_suffix = ".zip";
    std::string path;
    for (int i = 0; i < num; i++) {
        std::string file_name = std::to_string(i) + file_name_suffix;
        path = dir_path + file_name;
        printf("file path: %s\n", path.c_str());
        set_file_request(path.c_str());
        recv_file_write();
    }
    close_fd();
}

void FTClient::set_network() {
    file_buff = NULL;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_port = htons(8888);
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_family = AF_INET;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    int res = connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
    if (res < 0) {
        printf("connect error\n");
        exit(1);
    }
}

void FTClient::set_file_request(const char* file_path) {
    if (file_path != NULL) {
        write_file_path = file_path;
    }
    remove(write_file_path);
    filefd = open(write_file_path, O_WRONLY | O_CREAT);
    if (filefd == -1) {
        printf("open file error\n");
        exit(1);
    }
    request.length = strlen(request_file_path);
    memcpy(request.path, request_file_path, request.length);

    char buf[512] = { 0 };
    size_t bytes = 0;
    bytes = send(sockfd, (void*)&request, request.length + sizeof(int), 0);
    printf("ret: %ld\n", bytes);
    if (bytes < 0) {
        printf("send error\n");
        close(sockfd);
        exit(0);
    }

}
void FTClient::recv_file_write() {
    ssize_t read_bytes = 0;
    ssize_t bytes = 0;
    bool head = true;
    while (true) {
        if (head) {
            bytes = read(sockfd, &response + read_bytes, sizeof(int) - read_bytes);
            if (bytes == 0) {
                printf("server disconnect\n");
                break;
            }

            if (bytes < 0) {
                printf("read head error\n");
                break;
            }
            read_bytes += bytes;
            if (read_bytes == sizeof(int)) {
                if (response.size <= 0) {
                    printf("file path error\n");
                    break;
                }
                file_buff = new char[response.size];
                read_bytes = 0;
                head = false;
            }
            continue;
        }
        bytes = read(sockfd, file_buff + read_bytes, response.size - read_bytes);
        if (bytes == 0) {
            printf("server disconnect\n");
            break;
        }
        if (bytes < 0) {
            printf("read file error\n");
            break;
        }
        read_bytes += bytes;
        if (read_bytes == response.size) {
            break;
        }
    }

    if (file_buff != NULL) {
        ssize_t write_bytes = 0;
        while (write_bytes < response.size) {
            bytes = write(filefd, file_buff + write_bytes, response.size - write_bytes);
            if (bytes < 0) {
                printf("write file error\n");
                break;
            }
            write_bytes += bytes;
        }
    }
    close(filefd);
}
void FTClient::close_fd() {
    delete[] file_buff;
    close(filefd);
    close(sockfd);
    if (file_buff == NULL) {
        remove(write_file_path);
    }
    file_buff = NULL;
}

void FTClient::multy_connect_run(int num) {
    // 初始化网络socket
    file_buff = NULL;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_port = htons(8888);
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_family = AF_INET;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    int res = connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
    if (res < 0) {
        printf("connect error\n");
        exit(1);
    }

    // 设置文件部分
    std::string dir_path = "/mnt/e/resource/dest/";
    std::string file_name_suffix = ".zip";
    std::string path = dir_path + std::to_string(num) + file_name_suffix;
    write_file_path = path.c_str();
    
    remove(write_file_path);
    filefd = open(write_file_path, O_WRONLY | O_CREAT);
    if (filefd == -1) {
        printf("open file error\n");
        exit(1);
    }
    request.length = strlen(request_file_path);
    memcpy(request.path, request_file_path, request.length);

    char buf[512] = { 0 };
    size_t bytes = 0;
    bytes = send(sockfd, (void*)&request, request.length + sizeof(int), 0);
    printf("ret: %ld\n", bytes);
    if (bytes < 0) {
        printf("send error\n");
        close(sockfd);
        exit(0);
    }

    // 接收文件部分
    ssize_t read_bytes = 0;
    bytes = 0;
    bool head = true;
    while (true) {
        if (head) {
            bytes = read(sockfd, &response + read_bytes, sizeof(int) - read_bytes);
            if (bytes == 0) {
                printf("server disconnect\n");
                break;
            }

            if (bytes < 0) {
                printf("read head error\n");
                break;
            }
            read_bytes += bytes;
            if (read_bytes == sizeof(int)) {
                if (response.size <= 0) {
                    printf("file path error\n");
                    break;
                }
                file_buff = new char[response.size];
                read_bytes = 0;
                head = false;
            }
            continue;
        }
        bytes = read(sockfd, file_buff + read_bytes, response.size - read_bytes);
        if (bytes == 0) {
            printf("server disconnect\n");
            break;
        }
        if (bytes < 0) {
            printf("read file error\n");
            break;
        }
        read_bytes += bytes;
        if (read_bytes == response.size) {
            break;
        }
    }

    if (file_buff != NULL) {
        ssize_t write_bytes = 0;
        while (write_bytes < response.size) {
            bytes = write(filefd, file_buff + write_bytes, response.size - write_bytes);
            if (bytes < 0) {
                printf("write file error\n");
                break;
            }
            write_bytes += bytes;
        }
    }
    delete[] file_buff;
    close(filefd);
    close(sockfd);
    if (file_buff == NULL) {
        remove(write_file_path);
    }
    file_buff = NULL;
}