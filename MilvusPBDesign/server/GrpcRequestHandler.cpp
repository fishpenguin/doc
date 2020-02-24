#include "GrpcRequestHandler.h"

::demo::Status
GrpcRequestHandler::Search(::grpc::ClientContext* context, const ::demo::QueryRequestPB& request, ::demo::QueryResponsePB* response) {
    std::cout << "Search received!" << std::endl;
    ::demo::Status status;
    return status;
}