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
    INVALID = 0,
    R1,
    R2,
    R3,
    R4,
    AND,
    OR,
};

struct QueryColumn {
    std::string name;
    std::string column_value;
};

struct TermQuery {
    std::string field_name;
    std::vector<std::string> field_value;
    float boost;
};
using TermQueryPtr = std::shared_ptr<TermQuery>();

struct CompareExpr {
    CompareOperator compare_operator;
    std::string operand;
};

struct RangeQuery {
    std::string field_name;
    std::vector<CompareExpr> compare_expr;
    float boost;
};
using RangeQueryPtr = std::shared_ptr<RangeQuery>();

struct VectorRecord {
    std::vector<float> float_data;
    std::vector<uint8_t> binary_data;
};

struct VectorQuery {
    std::string field_name;
    std::string vector_query_param;
    uint64_t topk;
    float boost;
    std::vector<VectorRecord> query_vector;
};
using VectorQueryPtr = std::shared_ptr<VectorQuery>();

struct LeafQuery;
using LeafQueryPtr = std::shared_ptr<LeafQuery>;

struct BinaryQuery;
using BinaryQueryPtr = std::shared_ptr<BinaryQuery>;

struct GeneralQuery {
    LeafQueryPtr leaf;
    BinaryQueryPtr bin = std::make_shared<BinaryQuery>();
};
using GeneralQueryPtr = std::shared_ptr<GeneralQuery>;

struct LeafQuery {
    TermQuery term_query;
    RangeQuery range_query;
    VectorQuery vector_query;
    float query_boost;
};

struct BinaryQuery {
    GeneralQueryPtr left_query;
    GeneralQueryPtr right_query;
    QueryRelation relation;
    float query_boost;
};
