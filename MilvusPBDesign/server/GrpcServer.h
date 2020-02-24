#pragma once

#include "Status.h"

#include <grpcpp/grpcpp.h>
#include <cstdint>
#include <memory>
#include <string>
#include <thread>

class GrpcServer {
 public:
    static GrpcServer&
    GetInstance() {
        static GrpcServer grpc_server;
        return grpc_server;
    }

    void
    Start();
    void
    Stop();

 private:
    GrpcServer() = default;
    ~GrpcServer() = default;

    Status
    StartService();
    Status
    StopService();

 private:
    std::unique_ptr<::grpc::Server> server_ptr_;
    std::shared_ptr<std::thread> thread_ptr_;
};
