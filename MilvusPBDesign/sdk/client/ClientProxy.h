#pragma once

#include "CommonApi.h"
#include "BooleanQuery.h"
#include "GrpcClient.h"
#include "grpc-gen/gen-common/common.grpc.pb.h"

#include <memory>
#include <string>
#include <vector>

#include <iomanip>
#include <ctime>
#include <chrono>

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

template <typename T>
void
ConvertBinaryQueryToProto(GeneralQueryPtr<T>& general_query, ::demo::GeneralQueryPB* query_pb) {
    if (general_query->leaf == nullptr) {
        if(general_query->bin->left_query != nullptr) {
            ConvertBinaryQueryToProto<T>(general_query->bin->left_query, query_pb->mutable_bin()->mutable_left_query());
        }
        if(general_query->bin->right_query != nullptr) {
            ConvertBinaryQueryToProto<T>(general_query->bin->right_query, query_pb->mutable_bin()->mutable_right_query());
        }
        query_pb->mutable_bin()->set_relation((::demo::QueryRelation)(general_query->bin->relation));
    } else {
        query_pb->mutable_leaf()->set_boost(general_query->leaf->query_boost);
        if(!(general_query->leaf->query->term_query.field_name.empty())) {
            query_pb->mutable_leaf()->mutable_query()->mutable_term_query()->set_field_name(general_query->leaf->query->term_query.field_name);
            //OMG, need to judge T type, then decide to use which set function
            for (uint64_t i = 0; i < general_query->leaf->query->term_query.field_value.size(); ++i) {
                auto value =  query_pb->mutable_leaf()->mutable_query()->mutable_term_query()->add_values();
                value->set_int64_value(general_query->leaf->query->term_query.field_value[i]);
            }
            std::cout << "term" << std::endl;
        }
        if(!(general_query->leaf->query->range_query.field_name.empty())) {

            std::cout << "range" << std::endl;
            //assign range
        }
        if(!(general_query->leaf->query->vector_query.field_name.empty())) {
            //assign vector
            auto vector_query = query_pb->mutable_leaf()->mutable_query()->mutable_vector_query();
            vector_query->set_field_name(general_query->leaf->query->vector_query.field_name);
            vector_query->set_nq(general_query->leaf->query->vector_query.nq);
            vector_query->set_topk(general_query->leaf->query->vector_query.topk);
            vector_query->set_vector_query_param(general_query->leaf->query->vector_query.vector_query_param);
            uint64_t size = general_query->leaf->query->vector_query.query_vector.size();
            for (uint64_t i = 0; i < size; ++i) {
                ::demo::RowRecordPB *record = (*vector_query).add_records();
                record->mutable_data()->Resize(static_cast<int>(general_query->leaf->query->vector_query.query_vector[i].size()),
                                               0.0);
                memcpy(record->mutable_data()->mutable_data(),
                       general_query->leaf->query->vector_query.query_vector[i].data(),
                       general_query->leaf->query->vector_query.query_vector[i].size() * sizeof(float));
            }
            std::cout << "vector" << std::endl;
        }
    }
}

class ClientProxy {
 public:
    Status
    Connect(const std::string& uri);

    Status
    Connect(const ConnectParam& param);

    Status
    CreateCollection(uint64_t id, CollectionSchema collection_schema);

    template <typename T>
    QueryResponse
    Query(BooleanClausePtr<T> boolean_clause) {
        BinaryQueryPtr<T> binary_query_ptr = std::make_shared<BinaryQuery<T>>();
        Status s = GenBinaryQuery(boolean_clause, binary_query_ptr);

        std::cout << "*********************************************************" << std::endl;
        print_time();

        //Convert BinaryQuery to proto::GeneralQueryPB
        ::demo::QueryRequestPB request;
//        ::demo::GeneralQueryPB query_pb;
        GeneralQueryPtr<T> general_query = std::make_shared<GeneralQuery<T>>();
        general_query->bin = binary_query_ptr;
        ConvertBinaryQueryToProto<T>(general_query, request.mutable_query());
        //id?????????????????
        request.set_request_id(1);

        print_time();
        std::cout << "*********************************************************" << std::endl;

        ::demo::QueryResponsePB response;
        s = client_ptr_->Search(request, response);
        QueryResponse query_response;
        uint64_t size = response.ids().size();
        query_response.ids.resize(size);
        query_response.distances.resize(size);
        query_response.score.resize(size);
        memcpy(query_response.ids.data(), response.ids().data(), sizeof(uint64_t) * size);
        memcpy(query_response.distances.data(), response.distance().data(), sizeof(float) * size);
        memcpy(query_response.score.data(), response.distance().data(), sizeof(float) * size);
        query_response.hits = response.hits();
        std::cout << "*********************************************************"<<std::endl;
        return query_response;
    }

 private:
    std::shared_ptr<::grpc::Channel> channel_;
    std::shared_ptr<GrpcClient> client_ptr_;
    bool connected_ = false;
};
