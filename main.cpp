#include <iostream>
#include "FTServerClient.h"

int main(int, char**) {
    std::cout << "Hello, world!\n";
    FTServerClient client;
    client.request_file();
    client.get_response();
}
