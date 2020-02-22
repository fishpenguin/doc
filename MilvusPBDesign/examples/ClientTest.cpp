#include "ClientTest.h"
#include "client/ClientProxy.h"
#include "client/BooleanQuery.h"
#include "include/CommonApi.h"

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
    auto clause1 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST);
    std::vector<uint64_t> field_value{10, 20, 30};
    TermQuery<uint64_t> tq1 = {"age", field_value};
    InnerLeafQuery<uint64_t> inner_tq1;
    inner_tq1.term_query = tq1;
    LeafQuery<uint64_t> lq1 = {inner_tq1, 1.0};
    clause1->AddLeafQueries(std::shared_ptr<LeafQuery<uint64_t>>(&lq1));
    auto final_clause = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST);
    final_clause->AddBooleanClauses(clause1);

    QueryResponse response = proxy->Query(final_clause);
}
