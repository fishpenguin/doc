#pragma once

#include "CommonApi.h"
#include "BooleanQuery.h"
#include "grpc-gen/gen-common/common.grpc.pb.h"

#include <memory>
#include <string>
#include <vector>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

class GrpcClient {
 public:
    explicit GrpcClient(std::shared_ptr<::grpc::Channel>& channel);

    virtual ~GrpcClient();

    ::demo::CollectionSchemaResponsePB
    CreateCollection(::demo::CreateCollectionRequestPB);

    Status
    Search(::demo::QueryRequestPB& request, ::demo::QueryResponsePB& response);

 private:
    std::unique_ptr<::demo::DemoService::Stub> stub_;
};