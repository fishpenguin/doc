#pragma once

#include "Status.h"
#include "GeneralQuery.h"

enum class Occur {
    MUST = 0,
    MUST_NOT,
    SHOULD,
};

template<typename T>
class BooleanClause {
 public:
    BooleanClause();

    Occur getOccur() {
        return occur_;
    }

    std::vector<std::shared_ptr<BooleanClause> > getBooleanClauses() {
        return boolean_clauses_;
    }
 
    BinaryQueryPtr<T> getBinaryQuery() {
        return binary_query_;
    }

    std::vector<LeafQueryPtr<T>> getLeafQueries() {
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
    std::vector<LeafQueryPtr<T>> leaf_queries_;
    BinaryQueryPtr<T> binary_query_;
};
template <typename T>
using BooleanClausePtr = std::shared_ptr<BooleanClause<T>>;

template<typename T>
Status
GenBinaryQuery(BooleanClausePtr<T> boolean_clause, BinaryQueryPtr<T> binary_query);
