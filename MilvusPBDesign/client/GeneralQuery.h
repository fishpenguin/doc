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
    CompareOperator compare_operator;
    T operand;
};

template <typename T>
struct RangeQuery {
    std::string field_name;
    std::vector<CompareExpr<T>> compare_expr;
};

struct VectorQuery {
    std::string field_name;
    std::string vector_query_param;
    uint64_t nq;
    uint64_t topk;
    float query_boost;
    // query vector???
};

template <typename T>
union InnerLeafQuery {
    TermQuery<T> term_query;
    RangeQuery<T> range_query;
    VectorQuery vector_query;
};

template <typename T>
struct LeafQuery;
template <typename T>
using LeafQueryPtr = std::shared_ptr<LeafQuery<T>>;

template <typename T>
struct BinaryQuery;
template <typename T>
using BinaryQueryPtr = std::shared_ptr<BinaryQuery<T>>;

template <typename T>
union GeneralQuery {
    LeafQueryPtr<T> leaf;
    BinaryQueryPtr<T> bin;
};

template<typename T>
struct LeafQuery {
    InnerLeafQuery<T> query;
    float query_boost;
};

template <typename T>
struct BinaryQuery {
    GeneralQuery<T> left_query;
    GeneralQuery<T> right_query;
    QueryRelation relation;
    float query_boost;
};
