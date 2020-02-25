#include "ClientTest.h"
#include "sdk/client/ClientProxy.h"
#include "sdk/client/BooleanQuery.h"
#include "sdk/include/CommonApi.h"

#include <iostream>
#include <memory>
#include <utility>
#include <vector>

void
ClientTest::Test(const std::string& address, const std::string& port) {
    ConnectParam conn{address, port};
    auto proxy = std::make_shared<ClientProxy>();
    Status s = proxy->Connect(conn);

    //Construct a BooleanClause
    auto final_clause = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST);
    auto clause1 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST);
    LeafQueryPtr<uint64_t> lq1 = std::make_shared<LeafQuery<uint64_t>>();
    InnerLeafQueryPtr<uint64_t> inner_tq1 = std::make_shared<InnerLeafQuery<uint64_t>>();
    std::vector<uint64_t> field_value{10, 20, 30};
    TermQuery<uint64_t> tq1 = {"age", field_value};
    inner_tq1->term_query = tq1;
    lq1->query = inner_tq1;
    lq1->query_boost = 1.0;
    clause1->AddLeafQueries(lq1);
    final_clause->AddBooleanClauses(clause1);

//    QueryResponse response = proxy->Query(final_clause);
}
