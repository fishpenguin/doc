#include "ClientTest.h"
#include "Client/ClientProxy.h"
#include "ClientTest.h/BooleanQuery.h"

#include <iostream>
#include <memory>
#include <utility>
#include <vector>

void Test(const std::string& address, const std::string& port) {
    ConnectParam conn{address, port};
    auto proxy = std::make_shared<ClientProxy>();
    proxy.connect(conn);

    //Construct a BooleanClause
    auto clause1 = std::shared_ptr<BooleanClause>(Occur::MUST);
    std::vector<uint64_t> field_value{10, 20, 30};
    LeafQueryPtr<uint64_t> lq1{new TermQuery{"age", field_value}, 1.0};
    clause1.AddLeafQuery(lq1);
    auto final_clause = std::shared_ptr<BooleanClause>(Occur::MUST);
    final_clause.AddBooleanClause(clause1);

    QueryResponse response = proxy.query(final_clause);
}
