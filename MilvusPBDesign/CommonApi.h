#include "GeneralQuery.h"

template <typename T>
struct VectorColumn {
    uint64_t dimension;
    float vector_column_value;
}

struct ColumnSchema {
    uint64_t id;
    std::string name;
    
}

struct CollectionSchema {
    uint64_t id;
    std::string name;
}

struct QueryResponse {
    uint64_t hits;
    std::vector<uint64_t> ids;
    std::vector<uint64_t> score;
    std::vector<uint64_t> distances;
}

struct QueryClause {
    std::vector<TermQuery> term_queries;
    std::vector<RangeQuery> range_queries;
    std::vector<VectorQuery> vector_queries;
    std::vector<QueryClause> must_queries;
    std::vector<QueryClause> must_not_queries;
    std::vector<QueryClause> should_queries;
}

class Connection {
 public:
    static std::shared_ptr<Connection>
    create();

    uint64_t CreateCollection();

    QueryResponse Query(QueryClause)
}