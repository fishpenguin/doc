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

template <typename T>
struct QueryColumn {
    std::string name;
    T column_value;
};

template <typename T>
struct TermQuery : Query {
    std::vector<T> field_value;
};
template <typename T>
using TermQueryPtr = std::shared_ptr<TermQuery<T>>();

template <typename T>
struct CompareExpr {
    CompareOperator compare_operator;
    T operand;
};

template <typename T>
struct RangeQuery : Query {
    std::vector<CompareExpr<T>> compare_expr;
};
template <typename T>
using RangeQueryPtr = std::shared_ptr<RangeQuery<T>>();

struct RowRecord {
    std::vector<float> float_data;
    std::vector<uint8_t> binary_data;
}

struct VectorQuery : Query {
    std::string vector_query_param;
    uint64_t topk;
    float query_boost;
    std::vector<RowRecord> query_vector;
};
template <typename T>
using VectorQueryPtr = std::shared_ptr<VectorQuery<T>>();

template <typename T>
struct LeafQuery {
    TermQueryPtr<T> term_query_ptr;
    RangeQueryPtr<T> range_query_ptr;
    VectorQueryPtr vector_query_ptr;
    float query_boost;
}
template <typename T>
using LeafQueryPtr = std::shared_ptr<LeafQuery<T>>();