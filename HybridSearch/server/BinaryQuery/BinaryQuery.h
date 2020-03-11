#pragma once

#include "Status.h"
#include "GeneralQuery.h"

enum class Occur {
    INVALID = 0,
    MUST,
    MUST_NOT,
    SHOULD,
};

class BooleanClause {
 public:
    BooleanClause() {}

    BooleanClause(Occur occur) : occur_(occur) {}

    Occur getOccur() {
        return occur_;
    }

    void SetOccur(Occur occur) {
        occur_ = occur;
    }

    void AddBooleanClause(std::shared_ptr<BooleanClause> boolean_clause) {
        boolean_clauses_.emplace_back(boolean_clause);
    }

    void AddLeafQuery(LeafQueryPtr leaf_query) {
        leaf_queries_.emplace_back(leaf_query);
    }

    void SetLeafQuery(std::vector<LeafQueryPtr> leaf_queries) {
        leaf_queries_ = leaf_queries;
    }

    std::vector<std::shared_ptr<BooleanClause> > getBooleanClauses() {
        return boolean_clauses_;
    }

    BinaryQueryPtr& getBinaryQuery() {
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
    Occur occur_ = Occur::INVALID;
    std::vector<std::shared_ptr<BooleanClause>> boolean_clauses_;
    std::vector<LeafQueryPtr> leaf_queries_;
    BinaryQueryPtr binary_query_ = std::make_shared<BinaryQuery>();
};
using BooleanClausePtr = std::shared_ptr<BooleanClause>;


BinaryQueryPtr
ConstructBinTree(std::vector<BooleanClausePtr> clauses, QueryRelation relation, uint64_t idx) {
    if(idx == clauses.size()) {
        return nullptr;
    } else if (idx == clauses.size() - 1) {
        return clauses[idx]->getBinaryQuery();
    } else {
        BinaryQueryPtr bquery = std::make_shared<BinaryQuery>();
        bquery->relation = relation;
        bquery->left_query = std::make_shared<GeneralQuery>();
        bquery->right_query = std::make_shared<GeneralQuery>();
        bquery->left_query->bin = clauses[idx]->getBinaryQuery();
        ++idx;
        bquery->right_query->bin = ConstructBinTree(clauses, relation, idx);
        return bquery;
    }
}

Status
ConstructLeafBinTree(std::vector<LeafQueryPtr> leaf_clauses, BinaryQueryPtr binary_query, uint64_t idx) {
    if(idx == leaf_clauses.size()) {
        return Status::OK();
    }
    binary_query->left_query = std::make_shared<GeneralQuery>();
    binary_query->right_query = std::make_shared<GeneralQuery>();
    if (leaf_clauses.size() == leaf_clauses.size() - 1) {
        binary_query->left_query->leaf = leaf_clauses[idx];
        ++idx;
        return Status::OK();
    } else if (idx == leaf_clauses.size() - 2) {
        binary_query->left_query->leaf = leaf_clauses[idx];
        ++idx;
        binary_query->right_query->leaf = leaf_clauses[idx];
        ++idx;
        return Status::OK();
    } else {
        binary_query->left_query->bin->relation = binary_query->relation;
        binary_query->right_query->leaf = leaf_clauses[idx];
        ++idx;
        return ConstructLeafBinTree(leaf_clauses, binary_query->left_query->bin, idx);
    }
}

Status GenBinaryQuery(BooleanClausePtr clause,
                      BinaryQueryPtr& binary_query) {
    if(clause->getBooleanClauses().size() == 0) {
        // Judge leafquery
        // Relation is ready
        if (binary_query->relation == QueryRelation::AND || binary_query->relation == QueryRelation::OR) {
            return ConstructLeafBinTree(clause->getLeafQueries(), binary_query, 0);
        } else {
            switch (clause->getOccur()){
                case Occur::MUST: {
                    binary_query->relation = QueryRelation::AND;
                    return GenBinaryQuery(clause, binary_query);
                }
                case Occur::MUST_NOT:
                case Occur::SHOULD: {
                    binary_query->relation = QueryRelation::OR;
                    return GenBinaryQuery(clause, binary_query);
                }
            }
        }
    }

    if(clause->getBooleanClauses().size() == 1) {
        auto bc = clause->getBooleanClauses()[0];
        binary_query->left_query = std::make_shared<GeneralQuery>();
        switch (bc->getOccur()) {
            case Occur::MUST: {
                binary_query->relation = QueryRelation::AND;
                Status s = GenBinaryQuery(bc, binary_query);
                return s;
            }
            case Occur::MUST_NOT:
            case Occur::SHOULD: {
                binary_query->relation = QueryRelation::OR;
                Status s = GenBinaryQuery(bc, binary_query);
                return s;
            }
        }
    }

    // Construct binary query for every single boolean query
    std::vector<BooleanClausePtr> must_clauses;
    std::vector<BooleanClausePtr> must_not_clauses;
    std::vector<BooleanClausePtr> should_clauses;
    Status s;
    for (auto& _clause : clause->getBooleanClauses()) {
        s = GenBinaryQuery(_clause, _clause->getBinaryQuery());
        if (_clause->getOccur() == Occur::MUST) {
            must_clauses.emplace_back(_clause);
        }
        else if(_clause->getOccur() == Occur::MUST_NOT) {
            must_not_clauses.emplace_back(_clause);
        }
        else {
            should_clauses.emplace_back(_clause);
        }
    }

    // Construct binary query for combine boolean queries
    BinaryQueryPtr must_bquery, should_bquery, must_not_bquery;
    uint64_t bquery_num = 0;
    if(must_clauses.size() > 1) {
        // Construct a must binary tree
        must_bquery = ConstructBinTree(must_clauses, QueryRelation::R1, 0);
        ++bquery_num;
    } else if (must_clauses.size() == 1) {
        must_bquery = must_clauses[0]->getBinaryQuery();
        ++bquery_num;
    }

    if(should_clauses.size() > 1) {
        //Construct a should binary tree
        should_bquery = ConstructBinTree(should_clauses, QueryRelation::R2, 0);
        ++bquery_num;
    } else if (should_clauses.size() == 1) {
        should_bquery = should_clauses[0]->getBinaryQuery();
        ++bquery_num;
    }

    if(must_not_clauses.size() > 1) {
        // Construct a must_not binary tree
        must_not_bquery = ConstructBinTree(must_not_clauses, QueryRelation::R1, 0);
        ++bquery_num;
    } else if (must_not_clauses.size() == 1) {
        must_not_bquery = must_not_clauses[0]->getBinaryQuery();
        ++bquery_num;
    }

    binary_query->left_query = std::make_shared<GeneralQuery>();
    binary_query->right_query = std::make_shared<GeneralQuery>();
    BinaryQueryPtr must_should_query = std::make_shared<BinaryQuery>();
    must_should_query->left_query = std::make_shared<GeneralQuery>();
    must_should_query->right_query = std::make_shared<GeneralQuery>();
    if(bquery_num == 3) {
        must_should_query->relation = QueryRelation::R3;
        must_should_query->left_query->bin = must_bquery;
        must_should_query->right_query->bin = should_bquery;
        binary_query->relation = QueryRelation::R1;
        binary_query->left_query->bin = must_should_query;
        binary_query->right_query->bin = must_not_bquery;
    } else if (bquery_num == 2) {
        if(must_bquery == nullptr) {
            binary_query->relation = QueryRelation::R3;
            binary_query->left_query->bin = must_not_bquery;
            binary_query->right_query->bin = should_bquery;
        } else if (should_bquery == nullptr) {
            binary_query->relation = QueryRelation::R4;
            binary_query->left_query->bin = must_bquery;
            binary_query->right_query->bin = must_not_bquery;
        } else {
            binary_query->relation = QueryRelation::R3;
            binary_query->left_query->bin = must_bquery;
            binary_query->right_query->bin = should_bquery;
        }
    } else {
        if(must_bquery != nullptr) {
            binary_query = must_bquery;
        } else if(should_bquery != nullptr) {
            binary_query = should_bquery;
        } else {
            binary_query = must_not_bquery;
        }
    }

    return Status::OK();
}
