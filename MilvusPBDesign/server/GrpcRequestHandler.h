#include <cstdint>
#include <map>
#include <memory>
#include <random>
#include <string>
#include <unordered_map>

#include "grpc/gen-common/common.grpc.pb.h"
#include "grpc/gen-status/status.pb.h"

class GrpcRequestHandler final : public ::demo::DemoService {
 public:
    GrpcRequestHandler();

    ::demo::Status
    Search(::grpc::ClientContext* context, const ::demo::QueryRequestPB& request, ::demo::QueryResponsePB* response);
}