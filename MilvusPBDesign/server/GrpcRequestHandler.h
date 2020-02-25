#include <cstdint>
#include <map>
#include <memory>
#include <random>
#include <string>
#include <unordered_map>

#include "grpc-gen/gen-common/common.grpc.pb.h"
#include "grpc-gen/gen-status/status.pb.h"

 class GrpcRequestHandler final : public ::demo::DemoService::Service {
 public:
    ::grpc::Status
    Search(::grpc::ServerContext* context, const ::demo::QueryRequestPB* request, ::demo::QueryResponsePB* response) override ;
};