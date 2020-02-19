#include "Status.h"
#include "GeneralQuery.h"

enum class Occur {
    MUST = 0,
    MUST_NOT,
    SHOULD,
};

class BooleanClause {
 public:
    void add();

    Occur getOccur() {
        return occur_;
    }

    std::vector<std::shared_ptr<BooleanClause> > getBooleanClauses() {
        return boolean_clauses_;
    }
 
    BinaryQueryPtr getBinaryQuery() {
        return binary_query_;
    }

    std::vector<LeafQueryPtr> getLeafQueries() {
        return leaf_queries_;
    }

    bool isMust() {
        return occur_ == Occur::MUST;
    }

    bool isMustNot() {
        return occur_ == Occur::MUST_NOT;
    }

    bool isShould() {
        return occur_ == Occur::SHOULD;
    }

 private:
    Occur occur_;
    std::vector<std::shared_ptr<BooleanClause> > boolean_clauses_;
    std::vector<LeafQueryPtr> leaf_queries_;
    BinaryQueryPtr binary_query_;
}
using BooleanClausePtr = std::shared_ptr<BooleanClause>;


class BooleanQuery {
 public:
    void add(BooleanClause boolean_clause) {
        clauses_.emplace_back(boolean_clause);
    }

    Status GenBinaryQuery(BooleanClausePtr clauses, BinaryQueryPtr binary_query);
    
 private:
    std::vector<BooleanClause> clauses_;
}
