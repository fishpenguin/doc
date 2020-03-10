#include <iostream>
#include <memory>
#include <vector>
#include <string>

// base class of all queries
struct Sort {
    string field_name;
    int64_t rules;      // 0 is inc, 1 is dec
}

struct Query {
    string field_name;
    int64_t from;
    int64_t size;
    Sort sort;
    float min_score;
    int64_t boost;
}

enum class CompareOperator {
    LT = 0,
    LTE,
    EQ,
    GT,
    GTE,
    NE,
};

struct QueryColumn {
    std::string name;
    std::string column_value;
};

struct TermQuery : Query {
    std::vector<std::string> field_value;
};
using TermQueryPtr = std::shared_ptr<TermQuery>();

struct CompareExpr {
    CompareOperator compare_operator;
    std::string operand;
};

struct RangeQuery : Query {
    std::vector<CompareExpr> compare_expr;
};
using RangeQueryPtr = std::shared_ptr<RangeQuery>();

struct RowRecord {
    std::vector<float> float_data;
    std::vector<uint8_t> binary_data;
}

struct VectorQuery : Query {
    uint64_t topk;
    float query_boost;
    std::vector<RowRecord> query_vector;
    std::string extra_params;
};
using VectorQueryPtr = std::shared_ptr<VectorQuery>();

struct LeafQuery {
    TermQueryPtr term_query_ptr;
    RangeQueryPtr range_query_ptr;
    VectorQueryPtr vector_query_ptr;
    float query_boost;
}
using LeafQueryPtr = std::shared_ptr<LeafQuery>();