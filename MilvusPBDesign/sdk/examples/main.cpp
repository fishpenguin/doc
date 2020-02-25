#include <getopt.h>
#include <libgen.h>
#include <cstring>
#include <string>

#include "ClientTest.h"

int
main() {
    std::string address = "127.0.0.1", port = "19540";
    
    ClientTest test;
    test.Test(address, port);

    printf("Client stop...\n");
    return 0;
}