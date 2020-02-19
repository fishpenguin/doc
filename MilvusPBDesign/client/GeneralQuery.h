#include <iostream>
#include <memory>
#include <vector>
#include <string>

enum class CompareOperator {
    LT = 0,
    LTE,
    EQ,
    GT,
    GTE,
    NE,
};

enum class QueryRelation {
    R1 = 0,
    R2,
    R3,
    R4,
    AND,
    OR,
};

template <typename T>
struct QueryColumn {
    std::string name;
    T column_value;
};

template <typename T>
struct TermQuery {
    std::string field_name;
    std::vector<T> field_value;
};

template <typename T>
struct CompareExpr {
    CompareOperator operator;
    T operand;
};

template <typename T>
struct RangeQuery {
    std::string field_name;
    std::vector<CompareExpr> compare_expr;
};

struct VectorQuery {
    std::string field_name;
    std::string vector_query_param;
    uint64_t nq;
    uint64_t topk;
    float query_boost;
    // query vector???
};

union InnerLeafQuery {
    TermQuery term_query;
    RangeQuery range_query;
    VectorQuery vector_query;
};

struct LeafQuery {
    InnerLeafQuery query;
    float query_boost;
};
using LeafQueryPtr = std::shared_ptr<LeafQuery>;

struct BinaryQuery {
    GeneralQuery left_query;
    GeneralQuery right_query;
    QueryRelation relation;
    float query_boost;
};
using BinaryQueryPtr = std::shared_ptr<BinaryQuery>;

union GeneralQuery {
    LeafQueryPtr leaf;
    BinaryQueryPtr bin;
};