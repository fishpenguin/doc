#pragma once

#include "CommonApi.h"
#include "GrpcClient.h"

#include <memory>
#include <string>
#include <vector>

class ClientProxy : public Connection {
 public:
    Status
    Connect(const std::string& uri);

    Status
    Connect(const ConnectParam& param);

    Status
    CreateCollection(uint64_t id, CollectionSchema collection_schema);

    template <typename T>
    QueryResponse
    Query(BooleanClausePtr<T> boolean_clause);

 private:
    std::shared_ptr<::grpc::Channel> channel_;
    std::shared_ptr<GrpcClient> client_ptr_;
    bool connected_ = false;
};