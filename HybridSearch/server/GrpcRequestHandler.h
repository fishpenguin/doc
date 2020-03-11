#include <cstdint>
#include <map>
#include <memory>
#include <random>
#include <string>
#include <unordered_map>

#include "grpc/grpc-gen/milvus.grpc.pb.h"
#include "grpc/grpc-gen/status.pb.h"

 class GrpcRequestHandler final : public ::demo::DemoService::Service {
 public:
     ::grpc::Status
     Search(::grpc::ServerContext* context, const ::demo::SearchParam* request, ::demo::QueryResult* response) override ;
};