#include "Status.h"
#include "GeneralQuery.h"
#include <vector>

enum class Occur {
    uint64_t MUST = 0;
    uint64_t MUST_NOT;
    uint64_t SHOULD;
}

class BooleanClause {
 public:
    add();

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
        return occur_ == MUST;
    }

    bool isMustNot() {
        return occur_ == MUST_NOT;
    }

    bool isShould() {
        return occur_ == SHOULD;
    }

 private:
    Occur occur_;
    std::vector<std::shared_ptr<BooleanClause> > boolean_clauses_;
    std::vector<LeafQueryPtr> leaf_queries_;
    BinaryQueryPtr binary_query_;
}
std::shared_ptr<BooleanClause> BooleanClausePtr;


class BooleanQuery {
 public:
    void add(BooleanClause clause) {
        clauses_.emplace_bach(clause);
    }

    void getLeafQueries() {
        return leaf_queries;
    }

    BinaryQuery GenBinaryQuery(std::vector<BooleanClause> clauses);
    
 private:
    std::vector<LeafQueryPtr> leaf_queries;    
}
