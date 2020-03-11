#include <getopt.h>
#include <unistd.h>
#include <csignal>
#include <cstring>
#include <string>

#include "GrpcServer.h"

int main() {
//    GrpcServer::GetInstance().Start();
    std::cout<<"Milvuspb server started!" << std::endl;
    GrpcServer::GetInstance().Start();
}
