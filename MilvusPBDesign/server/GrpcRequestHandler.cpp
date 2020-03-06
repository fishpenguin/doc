#include <iomanip>
#include <ctime>
#include <chrono>
#include <queue>

#include "GrpcRequestHandler.h"

void print_time() {
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
}

void SequentialTraverse(const ::demo::QueryRequestPB* tree) {
    std::cout << "*****************************************************" << std::endl;

    std::queue<::demo::BinaryQueryPB> q;
    if (tree->has_query()) {
        if (tree->query().has_bin()){
            q.push(tree->query().bin());
        }
    }
    while (q.empty() == false) {
        std::cout << q.front().relation() << std::endl;
        if (q.front().has_left_query()){
            if (q.front().left_query().has_bin()) {
                q.push(q.front().left_query().bin());
            } else if (q.front().left_query().has_leaf()) {
                std::cout<< "left_query's leaf query boost: " << q.front().left_query().leaf().boost() << std::endl;
            }
        }
        if (q.front().has_right_query()) {
            if (q.front().right_query().has_bin()) {
                q.push(q.front().right_query().bin());
            } else if (q.front().right_query().has_leaf()) {
                std::cout << "right_query's leaf query boost: " << q.front().right_query().leaf().boost() <<std::endl;
            }
        }
        q.pop();
    }
    std::cout << "*****************************************************" << std::endl;
}

::grpc::Status
GrpcRequestHandler::Search(::grpc::ServerContext* context, const ::demo::QueryRequestPB* request, ::demo::QueryResponsePB* response) {
    print_time();
    SequentialTraverse(request);
    return ::grpc::Status::OK;
}