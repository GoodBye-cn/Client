#include <iostream>
#include "FTClient.h"

#include <pthread.h>

void* mulity_connect(void* arg) {
    FTClient client;
    client.multy_connect_run(*(int*)arg);
}

int main(int, char**) {
    std::cout << "Hello, world!\n";
    FTClient client;
    // client.run(7);
    pthread_t tids[5];
    int pthread_args[5] = { 1,2,3,4,5 };
    for (int i = 0; i < 5; i++) {
        pthread_create(&tids[i], NULL, mulity_connect, &pthread_args[i]);
    }
    for (int i = 0; i < 5; i++) {
        pthread_join(tids[i], NULL);
    }
    return 0;
}
