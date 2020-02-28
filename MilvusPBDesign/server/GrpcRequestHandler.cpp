#include <iomanip>
#include <ctime>
#include <chrono>

#include "GrpcRequestHandler.h"

::grpc::Status
GrpcRequestHandler::Search(::grpc::ServerContext* context, const ::demo::QueryRequestPB* request, ::demo::QueryResponsePB* response) {
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
              << microseconds.count() << std::endl;

    uint64_t size = request->query().bin().right_query().leaf().query().vector_query().records().size();
    uint64_t dimension = request->query().bin().right_query().leaf().query().vector_query().records()[0].data_size();
    float first = request->query().bin().right_query().leaf().query().vector_query().records()[0].data(1);
    std::cout << "Search received " << size << " vectors" << std::endl;
    std::cout << dimension << "+" << first << std::endl;
    return ::grpc::Status::OK;
}