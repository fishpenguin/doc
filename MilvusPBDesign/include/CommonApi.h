#pragma once

#include "client/BooleanQuery.h"
#include "Status.h"

#include <memory>
#include <string>
#include <vector>

struct ConnectParam {
    std::string ip_address;  ///< Server IP address
    std::string port;        ///< Server PORT
};

template <typename T>
struct VectorColumn {
    uint64_t dimension;
    float vector_column_value;
};

struct ColumnSchema {
    uint64_t id;
    std::string name;
    std::string column_type;
};

struct CollectionSchema {
    uint64_t id;
    std::string name;
    std::vector<ColumnSchema> columns;
};

struct QueryResponse {
    uint64_t hits;
    std::vector<uint64_t> ids;
    std::vector<uint64_t> score;
    std::vector<uint64_t> distances;
};

//struct QueryClause {
//    std::vector<TermQuery> term_queries;
//    std::vector<RangeQuery> range_queries;
//    std::vector<VectorQuery> vector_queries;
//    std::vector<QueryClause> must_queries;
//    std::vector<QueryClause> must_not_queries;
//    std::vector<QueryClause> should_queries;
//};
//
//class Connection {
// public:
//    virtual Status
//    Connect(const std::string uri);
//
//    virtual Status
//    Connect(const ConnectParam& connect_param);
//
//    virtual Status
//    CreateCollection(uint64_t request_id,
//                     CollectionSchema collection_schema);

//    template<typename T>
//    QueryResponse
//    Query(BooleanClause<T> query_clause);
//};