// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: common.proto

#include "common.pb.h"
#include "common.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/method_handler_impl.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace demo {

static const char* DemoService_method_names[] = {
  "/demo.DemoService/CreateTable",
  "/demo.DemoService/Search",
};

std::unique_ptr< DemoService::Stub> DemoService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< DemoService::Stub> stub(new DemoService::Stub(channel));
  return stub;
}

DemoService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_CreateTable_(DemoService_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Search_(DemoService_method_names[1], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status DemoService::Stub::CreateTable(::grpc::ClientContext* context, const ::demo::CreateTableRequestPB& request, ::demo::CreateTableResponsePB* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_CreateTable_, context, request, response);
}

void DemoService::Stub::experimental_async::CreateTable(::grpc::ClientContext* context, const ::demo::CreateTableRequestPB* request, ::demo::CreateTableResponsePB* response, std::function<void(::grpc::Status)> f) {
  return ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_CreateTable_, context, request, response, std::move(f));
}

void DemoService::Stub::experimental_async::CreateTable(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::demo::CreateTableResponsePB* response, std::function<void(::grpc::Status)> f) {
  return ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_CreateTable_, context, request, response, std::move(f));
}

::grpc::ClientAsyncResponseReader< ::demo::CreateTableResponsePB>* DemoService::Stub::AsyncCreateTableRaw(::grpc::ClientContext* context, const ::demo::CreateTableRequestPB& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::demo::CreateTableResponsePB>::Create(channel_.get(), cq, rpcmethod_CreateTable_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::demo::CreateTableResponsePB>* DemoService::Stub::PrepareAsyncCreateTableRaw(::grpc::ClientContext* context, const ::demo::CreateTableRequestPB& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::demo::CreateTableResponsePB>::Create(channel_.get(), cq, rpcmethod_CreateTable_, context, request, false);
}

::grpc::Status DemoService::Stub::Search(::grpc::ClientContext* context, const ::demo::QueryRequestPB& request, ::demo::QueryResponsePB* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_Search_, context, request, response);
}

void DemoService::Stub::experimental_async::Search(::grpc::ClientContext* context, const ::demo::QueryRequestPB* request, ::demo::QueryResponsePB* response, std::function<void(::grpc::Status)> f) {
  return ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_Search_, context, request, response, std::move(f));
}

void DemoService::Stub::experimental_async::Search(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::demo::QueryResponsePB* response, std::function<void(::grpc::Status)> f) {
  return ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_Search_, context, request, response, std::move(f));
}

::grpc::ClientAsyncResponseReader< ::demo::QueryResponsePB>* DemoService::Stub::AsyncSearchRaw(::grpc::ClientContext* context, const ::demo::QueryRequestPB& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::demo::QueryResponsePB>::Create(channel_.get(), cq, rpcmethod_Search_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::demo::QueryResponsePB>* DemoService::Stub::PrepareAsyncSearchRaw(::grpc::ClientContext* context, const ::demo::QueryRequestPB& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::demo::QueryResponsePB>::Create(channel_.get(), cq, rpcmethod_Search_, context, request, false);
}

DemoService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      DemoService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< DemoService::Service, ::demo::CreateTableRequestPB, ::demo::CreateTableResponsePB>(
          std::mem_fn(&DemoService::Service::CreateTable), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      DemoService_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< DemoService::Service, ::demo::QueryRequestPB, ::demo::QueryResponsePB>(
          std::mem_fn(&DemoService::Service::Search), this)));
}

DemoService::Service::~Service() {
}

::grpc::Status DemoService::Service::CreateTable(::grpc::ServerContext* context, const ::demo::CreateTableRequestPB* request, ::demo::CreateTableResponsePB* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status DemoService::Service::Search(::grpc::ServerContext* context, const ::demo::QueryRequestPB* request, ::demo::QueryResponsePB* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace demo

