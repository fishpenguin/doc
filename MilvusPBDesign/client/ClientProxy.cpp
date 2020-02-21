#include "ClientProxy.h"
#include "BooleanQuery.h"
#include "grpc-gen/gen-common/common.grpc.pb.h"

#include <memory>
#include <string>
#include <vector>

Status
ClientProxy::Connect(const ConnectParam& param) {
    std::string uri = param.ip_address + ":" + param.port;

    channel_ = ::grpc::CreateChannel(uri, ::grpc::InsecureChannelCredentials());
    if (channel_ != nullptr) {
        connected_ = true;
        client_ptr_ = std::make_shared<GrpcClient>(channel_);
        return Status::OK();
    }

    std::string reason = "connect failed!";
    connected_ = false;
    return Status(StatusCode::NotConnected, reason);
}

Status
ClientProxy::Connect(const std::string& uri) {
    size_t index = uri.find_first_of(':', 0);

    ConnectParam param;
    param.ip_address = uri.substr(0, index);
    param.port = uri.substr(index + 1);

    return Connect(param);
}

Status 
ClientProxy::CreateCollection(uint64_t id, CollectionSchema collection_schema) {
    
}

template <typename T>
void
ConvertBinaryQueryToProto(BinaryQueryPtr<T> binary_query, ::demo::BinaryQueryPB query_pb) {
    query_pb.set_relation(binary_query->relation);
    ConvertBinaryQueryToProto(binary_query->left_query, query_pb.left_query());
    ConvertBinaryQueryToProto(binary_query->right_query, query_pb.right_query());
}

template <typename T>
QueryResponse
ClientProxy::Query(BooleanClausePtr<T> boolean_clause) {
    BinaryQueryPtr<T> binary_query_ptr;
    Status s = GenBinaryQuery(boolean_clause, binary_query_ptr);
    
    //Convert BinaryQuery to proto::GeneralQueryPB
    ::demo::GeneralQueryPB query_pb;
    ConvertBinaryQueryToProto(binary_query_ptr, query_pb);
}
