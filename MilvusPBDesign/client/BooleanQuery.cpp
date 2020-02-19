#include "BooleanQuery.h"

BinaryQueryPtr
ConstructBinTree(std::vector<BooleanClausePtr> clauses, QueryRelation relation) {
    if(clauses.empty()) {
        return nullptr;
    } else if(clauses.size() == 1) {
        return clauses[0]->getBinaryQuery();
    } else {
        BinaryQueryPtr bquery;
        bquery->relation = relation;
        bquery->left_query = clauses[clauses.size()-1]->getBinaryQuery();
        clauses.pop_back();
        bquery->right_query = ConstructBinTree(clauses, relation);
        return bquery;
    }
}

Status
ConstructLeafBinTree(std::vector<LeafQueryPtr> leaf_clauses, BinaryQueryPtr binary_query) {
    if(leaf_clauses.empty()) {
        return Status::OK();
    } else if (leaf_clauses.size() == 1) {
        binary_query->left_query = leaf_clauses[0];
        return Status::OK();
    } else {
        BinaryQueryPtr bquery;
        bquery->relation = binary_query->relation;
        bquery->left_query = leaf_clauses[leaf_clauses.size() - 1];
        leaf_clauses.pop_back();
        Status s = ConstructLeafBinTree(leaf_clauses, bquery->right_query); 
        return s;
    }
}

Status
BooleanQuery::GenBinaryQuery(BooleanClausePtr clause,
    BinaryQueryPtr binary_query) {
    if(clause->getBooleanClauses().size() == 0) {
        // Judge leafquery
        // Relation is ready
        return ConstructLeafBinTree(clause->getLeafQueries(), binary_query);
    }

    if(clause->getBooleanClauses().size() == 1) {
        auto bc = clause->getBooleanClauses()[0];
        switch (bc->getOccur()) {
            case Occur::MUST: {
                binary_query->relation = QueryRelation::AND;
                Status s = GenBinaryQuery(bc, binary_query->left_query);
                return s;
            }
            case Occur::MUST_NOT:
            case Occur::SHOULD: {
                binary_query->relation = QueryRelation::OR;
                Status s = GenBinaryQuery(bc, binary_query->left_query);
                return s;
            }
        }
    }

    // Construct binary query for every single boolean query
    std::vector<std::shared_ptr<BooleanClause> > must_clauses;
    std::vector<std::shared_ptr<BooleanClause> > must_not_clauses;
    std::vector<std::shared_ptr<BooleanClause> > should_clauses;
    for (auto& _clause : clause->getBooleanClauses()) {
        Status s = GenBinaryQuery(_clause, _clause->getBinaryQuery());
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
        must_bquery = ConstructBinTree(must_clauses, QueryRelation::R1);
        ++bquery_num;
    }
    if(should_clauses.size() > 1) {
        //Construct a should binary tree 
        should_bquery = ConstructBinTree(should_clauses, QueryRelation::R2);
        ++bquery_num;
    }
    if(must_not_clauses.size() > 1) {
        // Construct a must_not binary tree
        must_not_bquery = ConstructBinTree(must_not_clauses, QueryRelation::R1);
        ++bquery_num;
    }
    
    if(bquery_num == 3) {
        BinaryQueryPtr must_should_query;
        must_should_query->relation = QueryRelation::R3;
        must_should_query->left_query = must_bquery;
        must_should_query->right_query = should_bquery;
        binary_query->relation = QueryRelation::R1;
        binary_query->left_query = must_should_query;
        binary_query->right_query = must_not_bquery;
    } else if (bquery_num == 2) {
        if(must_bquery == nullptr) {
            binary_query->relation = QueryRelation::R3;
            binary_query->left_query = must_not_bquery;
            binary_query->right_query = should_bquery;
        } else if (should_bquery == nullptr) {
            binary_query->relation = QueryRelation::R4;
            binary_query->left_query = must_bquery;
            binary_query->right_query = should_bquery;
        } else {
            binary_query->relation = QueryRelation::R3;
            binary_query->left_query = must_bquery;
            binary_query->right_query = should_bquery;
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