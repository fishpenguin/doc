# include "GeneralQuery.h"

enum class Occur {
    INVALID = 0,
    MUST,
    MUST_NOT,
    SHOULD,
};

class BooleanQuery : Query {
 public:
    BooleanQuery() {}

    BooleanQuery(Occur occur) : occur_(occur) {}
    
    AddLeafQuery(LeafQueryPtr leaf_query) {
        leaf_queries.emplace_back(leaf_query);
    }

    AddBooleanQuery(Occur occur, std::shared_ptr<BooleanQuery> boolean_query) {
        boolean_queries.emplace_back(boolean_query);
    }

    Occur GetOccur() {
        return occur_;
    }

 private:
    Occur occur_;
    std:vector<std::shared_ptr<BooleanQuery>> boolean_queries_;
    std::vector<LeafQueryPtr> leaf_queries_;
}
using BooleanQueryPtr = std::shared_ptr<BooleanQuery>();