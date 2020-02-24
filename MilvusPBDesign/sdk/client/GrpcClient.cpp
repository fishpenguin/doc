#include "GrpcClient.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;


GrpcClient::GrpcClient(std::shared_ptr<::grpc::Channel>& channel)
    : stub_(::demo::DemoService::NewStub(channel)) {
}

GrpcClient::~GrpcClient() = default;

Status
GrpcClient::Search(::demo::QueryRequestPB& query_request_pb, ::demo::QueryResponsePB& query_response_pb) {
    ClientContext context;
    Status status;
    ::grpc::Status grpc_status = stub_->Search(&context, query_request_pb, &query_response_pb);

    if(!grpc_status.ok()) {
        std::cerr << "Search rpc failed!" << std::endl;
        status = Status(StatusCode::RPCFailed, grpc_status.error_message());
        return status; 
    }

    return Status::OK();
}