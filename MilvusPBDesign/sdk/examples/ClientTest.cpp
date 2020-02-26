#include "ClientTest.h"
#include "sdk/client/ClientProxy.h"
#include "sdk/client/BooleanQuery.h"
#include "sdk/include/CommonApi.h"

#include <iostream>
#include <memory>
#include <utility>
#include <vector>

std::string general_field_name = "age";
std::string vector_field_name = "face_img";
std::shared_ptr<ClientProxy> proxy;


std::vector<LeafQueryPtr<uint64_t>> GenLeafQuery() {
    //Construct TermQuery
    std::vector<uint64_t> field_value{10, 20, 30};
    TermQuery<uint64_t> tq = {general_field_name, field_value};

    //Construct RangeQuery
    CompareExpr<uint64_t> ce1 = {CompareOperator::LTE, 20}, ce2 = {CompareOperator::GTE, 10};
    std::vector<CompareExpr<uint64_t>> ces{ce1, ce2};
    RangeQuery<uint64_t> rq = {general_field_name, ces};

    //Construct VectorQuery
    std::string vector_query_param = "vector_query_param";
    VectorQuery vq = {vector_field_name, vector_query_param, 100, 10, 1.0};

    std::vector<LeafQueryPtr<uint64_t>> lq;
    LeafQueryPtr<uint64_t> lq1 = std::make_shared<LeafQuery<uint64_t>>();
    LeafQueryPtr<uint64_t> lq2 = std::make_shared<LeafQuery<uint64_t>>();
    LeafQueryPtr<uint64_t> lq3 = std::make_shared<LeafQuery<uint64_t>>();
    lq.emplace_back(lq1);
    lq.emplace_back(lq2);
    lq.emplace_back(lq3);
    InnerLeafQueryPtr<uint64_t> inner_q1 = std::make_shared<InnerLeafQuery<uint64_t>>();
    InnerLeafQueryPtr<uint64_t> inner_q2 = std::make_shared<InnerLeafQuery<uint64_t>>();
    InnerLeafQueryPtr<uint64_t> inner_q3 = std::make_shared<InnerLeafQuery<uint64_t>>();
    inner_q1->term_query = tq;
    inner_q2->range_query = rq;
    inner_q3->vector_query = vq;

    lq1->query = inner_q1;
    lq2->query = inner_q2;
    lq3->query = inner_q3;
    lq1->query_boost = 1.0;
    return lq;
}

void
_1_query_case() {
    auto boolean_clause = std::make_shared<BooleanClause<uint64_t>>();
    auto leaf_queries = GenLeafQuery();
    //must
    auto must_clause = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST);
    must_clause->AddLeafQuery(leaf_queries[0]);
    must_clause->AddLeafQuery(leaf_queries[1]);
    must_clause->AddLeafQuery(leaf_queries[2]);
    auto must_response = proxy->Query(must_clause);
    //should
    auto should_clause = std::make_shared<BooleanClause<uint64_t>>(Occur::SHOULD);
    should_clause->SetLeafQuery(leaf_queries);
    auto should_response = proxy->Query(should_clause);
    //must_not
    auto must_not_clause = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST_NOT);
    must_not_clause->SetLeafQuery(leaf_queries);
    auto must_not_response = proxy->Query(must_not_clause);
}

void
_2_query_case() {
    auto leaf_queries = GenLeafQuery();
    //must+must
    auto mm = std::make_shared<BooleanClause<uint64_t>>();
    auto must_clause1 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST);
    auto must_clause2 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST);
    must_clause1->SetLeafQuery(leaf_queries);
    must_clause2->SetLeafQuery(leaf_queries);
    mm->AddBooleanClause(must_clause1);
    mm->AddBooleanClause(must_clause2);
    auto mm_response = proxy->Query(mm);

    //should+should
    auto ss = std::make_shared<BooleanClause<uint64_t>>();
    auto should_clause1 = std::make_shared<BooleanClause<uint64_t>>(Occur::SHOULD);
    auto should_clause2 = std::make_shared<BooleanClause<uint64_t>>(Occur::SHOULD);
    should_clause1->SetLeafQuery(leaf_queries);
    should_clause2->SetLeafQuery(leaf_queries);
    ss->AddBooleanClause(should_clause1);
    ss->AddBooleanClause(should_clause2);
    auto should_response = proxy->Query(ss);

    //must_not+must_not
    auto mnmn = std::make_shared<BooleanClause<uint64_t>>();
    auto must_not_clause1 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST_NOT);
    auto must_not_clause2 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST_NOT);
    must_not_clause1->SetLeafQuery(leaf_queries);
    must_not_clause2->SetLeafQuery(leaf_queries);
    mnmn->AddBooleanClause(must_not_clause1);
    mnmn->AddBooleanClause(must_not_clause2);
    auto mnmn_response = proxy->Query(mnmn);

    //must+should
    auto ms = std::make_shared<BooleanClause<uint64_t>>();
    ms->AddBooleanClause(must_clause1);
    ms->AddBooleanClause(should_clause1);
    auto ms_response = proxy->Query(ms);

    //must+must_not
    auto mmn = std::make_shared<BooleanClause<uint64_t>>();
    mmn->AddBooleanClause(must_clause1);
    mmn->AddBooleanClause(should_clause1);
    auto mmn_response = proxy->Query(mmn);

    //must_not+should
    auto mns = std::make_shared<BooleanClause<uint64_t>>();
    mns->AddBooleanClause(must_clause1);
    mns->AddBooleanClause(should_clause1);
    auto mns_response = proxy->Query(mns);
}

void
_3_query_case() {
    auto leaf_queries = GenLeafQuery();
    auto must_clause1 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST);
    auto must_clause2 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST);
    auto must_clause3 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST);
    must_clause1->SetLeafQuery(leaf_queries);
    must_clause2->SetLeafQuery(leaf_queries);
    must_clause3->SetLeafQuery(leaf_queries);

    auto should_clause1 = std::make_shared<BooleanClause<uint64_t>>(Occur::SHOULD);
    auto should_clause2 = std::make_shared<BooleanClause<uint64_t>>(Occur::SHOULD);
    auto should_clause3 = std::make_shared<BooleanClause<uint64_t>>(Occur::SHOULD);
    should_clause1->SetLeafQuery(leaf_queries);
    should_clause2->SetLeafQuery(leaf_queries);
    should_clause3->SetLeafQuery(leaf_queries);

    auto must_not_clause1 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST_NOT);
    auto must_not_clause2 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST_NOT);
    auto must_not_clause3 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST_NOT);
    must_not_clause1->SetLeafQuery(leaf_queries);
    must_not_clause2->SetLeafQuery(leaf_queries);
    must_not_clause3->SetLeafQuery(leaf_queries);

    //must+must+must
    auto mmm = std::make_shared<BooleanClause<uint64_t>>();
    mmm->AddBooleanClause(must_clause1);
    mmm->AddBooleanClause(must_clause2);
    mmm->AddBooleanClause(must_clause3);
    auto mmm_res = proxy->Query(mmm);

    //should+should+should
    auto sss = std::make_shared<BooleanClause<uint64_t>>();
    sss->AddBooleanClause(should_clause1);
    sss->AddBooleanClause(should_clause2);
    sss->AddBooleanClause(should_clause3);
    auto sss_res = proxy->Query(sss);

    //must_not+must_not+must_not
    auto mmmnnn = std::make_shared<BooleanClause<uint64_t>>();
    mmmnnn->AddBooleanClause(must_not_clause1);
    mmmnnn->AddBooleanClause(must_not_clause2);
    mmmnnn->AddBooleanClause(must_not_clause3);
    auto mmmnnn_res = proxy->Query(mmmnnn);

    //m+m+s
    auto mms = std::make_shared<BooleanClause<uint64_t>>();
    mms->AddBooleanClause(must_clause1);
    mms->AddBooleanClause(must_clause2);
    mms->AddBooleanClause(should_clause1);
    auto mms_res = proxy->Query(mms);

    //m+m+mn
    auto mmmn = std::make_shared<BooleanClause<uint64_t>>();
    mmmn->AddBooleanClause(must_clause1);
    mmmn->AddBooleanClause(must_clause2);
    mmmn->AddBooleanClause(must_not_clause1);
    auto mmmn_res = proxy->Query(mmmn);

    //s+s+m
    auto ssm = std::make_shared<BooleanClause<uint64_t>>();
    ssm->AddBooleanClause(should_clause1);
    ssm->AddBooleanClause(should_clause2);
    ssm->AddBooleanClause(must_clause3);
    auto ssm_res = proxy->Query(ssm);

    //s+s+mn
    auto ssmn = std::make_shared<BooleanClause<uint64_t>>();
    ssmn->AddBooleanClause(should_clause1);
    ssmn->AddBooleanClause(should_clause2);
    ssmn->AddBooleanClause(must_not_clause3);
    auto ssmn_res = proxy->Query(ssmn);

    //mn+mn+m
    auto mnmnm = std::make_shared<BooleanClause<uint64_t>>();
    mnmnm->AddBooleanClause(must_not_clause1);
    mnmnm->AddBooleanClause(must_not_clause2);
    mnmnm->AddBooleanClause(must_clause1);
    auto mnmnm_res = proxy->Query(mnmnm);

    //mn+mn+s
    auto mnmns = std::make_shared<BooleanClause<uint64_t>>();
    mnmns->AddBooleanClause(must_not_clause1);
    mnmns->AddBooleanClause(must_not_clause2);
    mnmns->AddBooleanClause(should_clause3);
    auto mnmns_res = proxy->Query(mnmns);

    //m+s+mn
    auto msmn = std::make_shared<BooleanClause<uint64_t>>();
    msmn->AddBooleanClause(must_clause1);
    msmn->AddBooleanClause(should_clause1);
    msmn->AddBooleanClause(must_not_clause1);
    auto msmn_res = proxy->Query(msmn);
}


void
ClientTest::Test(const std::string& address, const std::string& port) {
    ConnectParam conn{address, port};
    proxy = std::make_shared<ClientProxy>();
    Status s = proxy->Connect(conn);
 

//    _1_query_case();

    _2_query_case();

    //Construct a BooleanClause
//     auto final_clause = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST);
//     auto clause1 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST);
//     LeafQueryPtr<uint64_t> lq1 = std::make_shared<LeafQuery<uint64_t>>();
//     InnerLeafQueryPtr<uint64_t> inner_tq1 = std::make_shared<InnerLeafQuery<uint64_t>>();
//     std::vector<uint64_t> field_value{10, 20, 30};
//     TermQuery<uint64_t> tq1 = {"age", field_value};
//     inner_tq1->term_query = tq1;
//     lq1->query = inner_tq1;
//     lq1->query_boost = 1.0;
//     clause1->AddLeafQuery(lq1);
//     final_clause->AddBooleanClause(clause1);

//    QueryResponse response = proxy->Query(final_clause);
}
