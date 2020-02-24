#include "GrpcRequestHandler.h"

::grpc::Status
GrpcRequestHandler::Search(::grpc::ClientContext* context, const ::demo::QueryRequestPB& request, ::demo::QueryResponsePB* response) {
    std::cout << "Search received!" << std::endl;
    return ::grpc::Status::OK;
}