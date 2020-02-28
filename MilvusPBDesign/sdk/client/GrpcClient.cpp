#include <iomanip>
#include <ctime>
#include <chrono>

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

    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();

    typedef std::chrono::duration<int, std::ratio_multiply<std::chrono::hours::period, std::ratio<8>>::type> Days; /* UTC: +8:00 */

    Days days = std::chrono::duration_cast<Days>(duration);
    duration -= days;
    auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
    duration -= hours;
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);
    duration -= minutes;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
    duration -= seconds;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    duration -= milliseconds;
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration);
    duration -= microseconds;
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);

    std::cout << hours.count() << ":"
              << minutes.count() << ":"
              << seconds.count() << ":"
              << milliseconds.count() << ":"
              << microseconds.count() << ":"
              << nanoseconds.count() << std::endl;

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    ::grpc::Status grpc_status = stub_->Search(&context, query_request_pb, &query_response_pb);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "transport time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

    now = std::chrono::system_clock::now();
    duration = now.time_since_epoch();

    typedef std::chrono::duration<int, std::ratio_multiply<std::chrono::hours::period, std::ratio<8>>::type> Days; /* UTC: +8:00 */

    days = std::chrono::duration_cast<Days>(duration);
    duration -= days;
    hours = std::chrono::duration_cast<std::chrono::hours>(duration);
    duration -= hours;
    minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);
    duration -= minutes;
    seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
    duration -= seconds;
    milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    duration -= milliseconds;
    microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration);
    duration -= microseconds;
    nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);

    std::cout << hours.count() << ":"
              << minutes.count() << ":"
              << seconds.count() << ":"
              << milliseconds.count() << ":"
              << microseconds.count() << std::endl;

    if(!grpc_status.ok()) {
        std::cerr << "Search rpc failed!" << std::endl;
        status = Status(StatusCode::RPCFailed, grpc_status.error_message());
        return status; 
    }

    return Status::OK();
}