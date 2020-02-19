

enum class CompareOperator {
    uint64_t LT = 0;
    uint64_t LTE;
    uint64_t EQ;
    uint64_t GT;
    uint64_t GTE;
    uint64_t NE;
}

enum class QueryRelation {
    uint64_t R1 = 0;
    uint64_t R2;
    uint64_t R3;
    uint64_t R4;
    uint64_t AND;
    uint64_t OR;
}

template <typename T>
struct QueryColumn {
    std::string name;
    T column_value;
}

template <typename T>
struct TermQuery {
    std::string field_name;
    std::vector<T> field_value;
}

template <typename T>
struct CompareExpr {
    CompareOperator operator;
    T operand;
}

template <typename T>
struct RangeQuery {
    std::string field_name;
    std::vector<CompareExpr> compare_expr;
}

struct VectorQuery {
    std::string field_name;
    std::string vector_query_param;
    uint64_t nq;
    uint64_t topk;
    float query_boost;
    // query vector???
}

union InnerLeafQuery {
    TermQuery term_query;
    RangeQuery range_query;
    VectorQuery vector_query;
}

struct LeafQuery {
    InnerLeafQuery query;
    float query_boost;
}
std::shared_ptr<LeafQuery> LeafQueryPtr;

class BinaryQuery {
private:
    GeneralQuery left_query;
    GeneralQuery right_query;
    QueryRelation relation;
    float query_boost;
}
std::shared_ptr<BinaryQuery> BinaryQueryPtr;

union GeneralQuery {
    LeafQueryPtr leaf;
    BinaryQueryPtr bin;
}