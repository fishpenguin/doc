#include "GrpcServer.h"

#include <grpc++/grpc++.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "GrpcRequestHandler.h"
#include "grpc-gen/gen-common/common.grpc.pb.h"

constexpr int64_t MESSAGE_SIZE = -1;

// this class is to check port occupation during server start
class NoReusePortOption : public ::grpc::ServerBuilderOption {
 public:
    void
    UpdateArguments(::grpc::ChannelArguments* args) override {
        args->SetInt(GRPC_ARG_ALLOW_REUSEPORT, 0);
        args->SetInt(GRPC_ARG_MAX_CONCURRENT_STREAMS, 20);
    }

    void
    UpdatePlugins(std::vector<std::unique_ptr<::grpc::ServerBuilderPlugin>>* plugins) override {
    }
};

void
GrpcServer::Start() {
//    thread_ptr_ = std::make_shared<std::thread>(&GrpcServer::StartService, this);
    GrpcServer::StartService();
}

void
GrpcServer::Stop() {
    StopService();
//    if (thread_ptr_) {
//        thread_ptr_->join();
//        thread_ptr_ = nullptr;
//    }
}

Status
GrpcServer::StartService() {
    std::string address = "127.0.0.1";
    std::string port = "19540";
    Status s;

    if (!s.ok()) {
        return s;
    }
    if (!s.ok()) {
        return s;
    }

    std::string server_address(address + ":" + port);

    ::grpc::ServerBuilder builder;
    builder.SetOption(std::unique_ptr<::grpc::ServerBuilderOption>(new NoReusePortOption));
    builder.SetMaxReceiveMessageSize(MESSAGE_SIZE);  // default 4 * 1024 * 1024
    builder.SetMaxSendMessageSize(MESSAGE_SIZE);

    builder.SetCompressionAlgorithmSupportStatus(GRPC_COMPRESS_STREAM_GZIP, true);
    builder.SetDefaultCompressionAlgorithm(GRPC_COMPRESS_STREAM_GZIP);
    builder.SetDefaultCompressionLevel(GRPC_COMPRESS_LEVEL_NONE);

    GrpcRequestHandler service;

    builder.AddListeningPort(server_address, ::grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    server_ptr_ = builder.BuildAndStart();
    server_ptr_->Wait();

    return Status::OK();
}

Status
GrpcServer::StopService() {
    if (server_ptr_ != nullptr) {
        server_ptr_->Shutdown();
    }

    return Status::OK();
}
