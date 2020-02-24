#include "ClientProxy.h"

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


//template <typename T>
//QueryResponse
//ClientProxy::Query(BooleanClausePtr<T> boolean_clause) {
//    BinaryQueryPtr<T> binary_query_ptr;
//    Status s = GenBinaryQuery(boolean_clause, binary_query_ptr);
//
//    //Convert BinaryQuery to proto::GeneralQueryPB
//    ::demo::GeneralQueryPB query_pb;
//    ConvertBinaryQueryToProto(binary_query_ptr, query_pb);
//    ::demo::QueryRequestPB request;
//    //id?????????????????
//    request.set_request_id(1);
//    request.set_allocated_query(&query_pb);
//    ::demo::QueryResponsePB response;
//    s = client_ptr_->Search(request, response);
//    QueryResponse query_response;
//    uint64_t size = response.ids().size();
//    query_response.ids.resize(size);
//    query_response.distances.resize(size);
//    query_response.score.resize(size);
//    memcpy(query_response.ids.data(), response.ids().data(), sizeof(uint64_t) * size);
//    memcpy(query_response.distances.data(), response.distance().data(), sizeof(float) * size);
//    memcpy(query_response.score.data(), response.distance().data(), sizeof(float) * size);
//    query_response.hits = response.hits();
//    return query_response;
//}
