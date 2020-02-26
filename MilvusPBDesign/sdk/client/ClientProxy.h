#pragma once

#include "CommonApi.h"
#include "BooleanQuery.h"
#include "GrpcClient.h"
#include "grpc-gen/gen-common/common.grpc.pb.h"

#include <memory>
#include <string>
#include <vector>

#include <memory>
#include <string>
#include <vector>

template <typename T>
void
ConvertBinaryQueryToProto(GeneralQueryPtr<T> general_query, ::demo::GeneralQueryPB* query_pb) {
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
//            query_pb->mutable_leaf()->mutable_query()->mutable_term_query()->set_field_name(general_query->leaf->)
            std::cout << "term" << std::endl;
        }
        if(!(general_query->leaf->query->range_query.field_name.empty())) {
            std::cout << "range" << std::endl;
            //assign range
        }
        if(!(general_query->leaf->query->vector_query.field_name.empty())) {
            //assign vector
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

        //Convert BinaryQuery to proto::GeneralQueryPB
        ::demo::QueryRequestPB request;
//        ::demo::GeneralQueryPB query_pb;
        GeneralQueryPtr<T> general_query = std::make_shared<GeneralQuery<T>>();
        general_query->bin = binary_query_ptr;
        ConvertBinaryQueryToProto<T>(general_query, request.mutable_query());
        //id?????????????????
        request.set_request_id(1);

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
