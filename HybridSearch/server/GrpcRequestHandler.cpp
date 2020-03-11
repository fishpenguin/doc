#include <iomanip>
#include <ctime>
#include <chrono>
#include <queue>

#include "GrpcRequestHandler.h"
#include "BinaryQuery/BinaryQuery.h"

void DeSerialization(const ::demo::GeneralQuery& general_query, BooleanClausePtr& boolean_clause) {
    if (general_query.has_boolean_query()) {
        boolean_clause->SetOccur(Occur(general_query.boolean_query().occur()));
        for (uint64_t i = 0; i < general_query.boolean_query().general_query_size(); ++i) {
            auto clause = std::make_shared<BooleanClause>();
            DeSerialization(general_query.boolean_query().general_query(i), clause);
            boolean_clause->AddBooleanClause(clause);
        }
    }

    auto leaf_clause = std::make_shared<LeafQuery>();
    if (general_query.has_term_query()) {
        TermQuery term_query;
        term_query.field_name = general_query.term_query().field_name();
        term_query.boost = general_query.term_query().boost();
        term_query.field_value.resize(general_query.term_query().values_size());
        for (uint64_t i = 0; i < general_query.term_query().values_size(); ++i) {
            term_query.field_value[i] = general_query.term_query().values(i);
        }
        leaf_clause->term_query = term_query;
    }
    if (general_query.has_range_query()) {
        RangeQuery range_query;
        range_query.field_name = general_query.range_query().field_name();
        range_query.boost = general_query.range_query().boost();
        range_query.compare_expr.resize(general_query.range_query().operand_size());
        for (uint64_t i = 0; i < general_query.range_query().operand_size(); ++i) {
            range_query.compare_expr[i].compare_operator =
                CompareOperator(general_query.range_query().operand(i).operator_());
            range_query.compare_expr[i].operand = general_query.range_query().operand(i).operand();
        }
        leaf_clause->range_query = range_query;
    }
    if (general_query.has_vector_query()) {
        VectorQuery vector_query;
        vector_query.boost = general_query.vector_query().query_boost();
        vector_query.field_name = general_query.vector_query().field_name();
        vector_query.topk = general_query.vector_query().topk();
        vector_query.query_vector.resize(general_query.vector_query().records_size());
        for (uint64_t i = 0; i < general_query.vector_query().records_size(); ++i) {
            vector_query.query_vector[i].float_data.resize(general_query.vector_query().records(i).float_data_size());
            vector_query.query_vector[i].binary_data.resize(general_query.vector_query().records(i).binary_data().size() / 8)
            memcpy(vector_query.query_vector[i].float_data.data(), general_query.vector_query().records(i).float_data().data(),
                   sizeof(float) * general_query.vector_query().records(i).float_data_size());
            memcpy(vector_query.query_vector[i].binary_data.data(),
                   general_query.vector_query().records(i).binary_data().data(),
                   general_query.vector_query().records(i).binary_data().size());
        }
        leaf_clause->vector_query = vector_query;
    }
    boolean_clause->AddLeafQuery(leaf_clause);
}

::grpc::Status
GrpcRequestHandler::Search(::grpc::ServerContext* context, const ::demo::SearchParam* request, ::demo::QueryResult* response) {
    std::string collection_name = request->collection_name();
    std::vector<std::string> partition_tag_array;
    partition_tag_array.resize(request->partition_tag_array_size());
    auto bool_clause = std::make_shared<BooleanClause>();
    for (uint64_t i = 0; i < request->partition_tag_array_size(); ++i) {
        partition_tag_array[i] = request->partition_tag_array(i);
    }
    // Construct a BinaryQuery


    return ::grpc::Status::OK;
}