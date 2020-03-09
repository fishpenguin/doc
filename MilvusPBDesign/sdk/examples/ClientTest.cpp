#include "ClientTest.h"
#include "sdk/client/ClientProxy.h"
#include "sdk/client/BooleanQuery.h"
#include "sdk/include/CommonApi.h"

#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include <iomanip>
#include <ctime>
#include <chrono>

std::string general_field_name = "age";
std::string vector_field_name = "face_img";
uint64_t NQ = 1000;
uint64_t TOPK = 10000;
uint64_t DIMENSION = 512;
std::shared_ptr<ClientProxy> proxy;

void ConstructVector(uint64_t nq, uint64_t dimension, std::vector<std::vector<float>>& query_vector) {
    query_vector.resize(nq);
    for (uint64_t i = 0; i < nq; ++i) {
        query_vector[i].resize(dimension);
        for (uint64_t j = 0; j < dimension; ++j) {
            query_vector[i][j] = (float)((i + 100) / (j + 1));
        }
    }
}

std::vector<LeafQueryPtr<uint64_t>> GenLeafQuery() {
    //Construct TermQuery
    std::vector<uint64_t> field_value{10, 20, 30};
    TermQuery<uint64_t> tq = {general_field_name, field_value};

    //Construct RangeQuery
    CompareExpr<uint64_t> ce1 = {CompareOperator::LTE, 20}, ce2 = {CompareOperator::GTE, 10};
    std::vector<CompareExpr<uint64_t>> ces{ce1, ce2};
    RangeQuery<uint64_t> rq = {general_field_name, ces};

    //Construct VectorQuery
    std::vector<std::vector<float >> query_vector;
    ConstructVector(NQ, DIMENSION, query_vector);
    std::string vector_query_param = "vector_query_param";
    VectorQuery vq = {vector_field_name, vector_query_param, NQ, TOPK, 1.0, query_vector};

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
    lq2->query_boost = 2.0;
    lq3->query_boost = 3.0;
    return lq;
}

std::vector<LeafQueryPtr<uint64_t>> GenSpecLeafQuery(uint64_t term_num, uint64_t range_num, uint64_t vector_num) {

    std::vector<TermQuery<uint64_t>> tqs;
    std::vector<RangeQuery<uint64_t>> rqs;
    std::vector<VectorQuery> vqs;
    tqs.resize(term_num);
    rqs.resize(range_num);
    vqs.resize(vector_num);

    std::vector<LeafQueryPtr<uint64_t>> lq;
    lq.resize(term_num + range_num + vector_num);

    std::vector<InnerLeafQueryPtr<uint64_t>> inner_queries;
    inner_queries.resize(term_num + range_num + vector_num);
    for (uint64_t i = 0; i < term_num + range_num + vector_num; ++i) {
        lq[i] = std::make_shared<LeafQuery<uint64_t>>();
        inner_queries[i] = std::make_shared<InnerLeafQuery<uint64_t>>();
    }

    uint64_t i = 0;
    for (i = 0; i < term_num; i++) {
        std::vector<uint64_t> field_value{10, 20, 30};
        TermQuery<uint64_t> tq = {general_field_name, field_value};
        inner_queries[i]->term_query = tq;
    }
    for (; i < term_num + range_num; ++i) {
        CompareExpr<uint64_t> ce1 = {CompareOperator::LTE, 20}, ce2 = {CompareOperator::GTE, 10};
        std::vector<CompareExpr<uint64_t>> ces{ce1, ce2};
        RangeQuery<uint64_t> rq = {general_field_name, ces};
        inner_queries[i]->range_query = rq;
    }
    for (; i < term_num + range_num + vector_num; ++i) {
        std::vector<std::vector<float >> query_vector;
        ConstructVector(NQ, DIMENSION, query_vector);
        std::string vector_query_param = "vector_query_param";
        VectorQuery vq = {vector_field_name, vector_query_param, NQ, TOPK, 1.0, query_vector};
        inner_queries[i]->vector_query = vq;
    }

    for (uint64_t j = 0; j < term_num + range_num + vector_num; ++j) {
        lq[j]->query = inner_queries[j];
        lq[j]->query_boost = j + 1;
    }

    return lq;
}

void test_performance() {
    std::vector<std::vector<float >> query_vector;
    uint64_t nq = 10000;
    uint64_t dimension = 512;
    ConstructVector(nq, dimension, query_vector);
    std::string vector_query_param = "vector_query_param";
    VectorQuery vq = {vector_field_name, vector_query_param, NQ, TOPK, 1.0, query_vector};
    LeafQueryPtr<uint64_t> lq = std::make_shared<LeafQuery<uint64_t>>();
    InnerLeafQueryPtr<uint64_t> inner_q = std::make_shared<InnerLeafQuery<uint64_t>>();
    inner_q->vector_query = vq;
    lq->query = inner_q;
    lq->query_boost = 1.0;

    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();

    typedef std::chrono::duration<int, std::ratio_multiply<std::chrono::hours::period, std::ratio<8>>::type> Days; /* UTC: +8:00 */

    Days days = std::chrono::duration_cast<Days>(duration);
    duration -= days;
    auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
    duration -= hours;
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);
    duration -= minutes;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
    duration -= seconds;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    duration -= milliseconds;
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration);
    duration -= microseconds;
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);

    std::cout << hours.count() << ":"
              << minutes.count() << ":"
              << seconds.count() << ":"
              << milliseconds.count() << ":"
              << microseconds.count() << std::endl;

    auto fff = std::make_shared<BooleanClause<uint64_t>>();
    auto ff = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST);
    auto final_clause = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST);
    auto boolean_clause = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST);
    //must
    auto must_clause = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST);
    must_clause->AddLeafQuery(lq);
    final_clause->AddBooleanClause(must_clause);
    ff->AddBooleanClause(final_clause);
    fff->AddBooleanClause(ff);
    auto must_response = proxy->Query(must_clause);
}

void
_1_query_case() {
    auto boolean_clause = std::make_shared<BooleanClause<uint64_t>>();
    auto leaf_queries = GenLeafQuery();

    // Gen leaf_query with different template
    std::vector<std::string> field_value{"10", "20", "30"};
    TermQuery<std::string> tq = {general_field_name, field_value};
    LeafQueryPtr<std::string> tlq = std::make_shared<LeafQuery<std::string>>();
    InnerLeafQueryPtr<std::string> tilq = std::make_shared<InnerLeafQuery<std::string>>();
    tilq->term_query = tq;
    tlq->query_boost = 123456;
    tlq->query = tilq;

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

void other_query_cases() {
    auto leaf_queries = GenLeafQuery();
    auto must_clause1 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST);
    auto must_clause2 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST);
    auto must_clause3 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST);
    auto must_clause4 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST);
    auto must_clause5 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST);
    auto must_clause6 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST);
    auto must_clause7 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST);
    auto must_clause8 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST);
    auto must_clause9 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST);
    auto must_clause10 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST);
    must_clause1->SetLeafQuery(leaf_queries);
    must_clause2->SetLeafQuery(leaf_queries);
    must_clause3->SetLeafQuery(leaf_queries);
    must_clause4->SetLeafQuery(leaf_queries);
    must_clause5->SetLeafQuery(leaf_queries);
    must_clause6->SetLeafQuery(leaf_queries);
    must_clause7->SetLeafQuery(leaf_queries);
    must_clause8->SetLeafQuery(leaf_queries);
    must_clause9->SetLeafQuery(leaf_queries);
    must_clause10->SetLeafQuery(leaf_queries);

    auto should_clause1 = std::make_shared<BooleanClause<uint64_t>>(Occur::SHOULD);
    auto should_clause2 = std::make_shared<BooleanClause<uint64_t>>(Occur::SHOULD);
    auto should_clause3 = std::make_shared<BooleanClause<uint64_t>>(Occur::SHOULD);
    auto should_clause4 = std::make_shared<BooleanClause<uint64_t>>(Occur::SHOULD);
    auto should_clause5 = std::make_shared<BooleanClause<uint64_t>>(Occur::SHOULD);
    auto should_clause6 = std::make_shared<BooleanClause<uint64_t>>(Occur::SHOULD);
    auto should_clause7 = std::make_shared<BooleanClause<uint64_t>>(Occur::SHOULD);
    auto should_clause8 = std::make_shared<BooleanClause<uint64_t>>(Occur::SHOULD);
    auto should_clause9 = std::make_shared<BooleanClause<uint64_t>>(Occur::SHOULD);
    auto should_clause10 = std::make_shared<BooleanClause<uint64_t>>(Occur::SHOULD);
    should_clause1->SetLeafQuery(leaf_queries);
    should_clause2->SetLeafQuery(leaf_queries);
    should_clause3->SetLeafQuery(leaf_queries);
    should_clause4->SetLeafQuery(leaf_queries);
    should_clause5->SetLeafQuery(leaf_queries);
    should_clause6->SetLeafQuery(leaf_queries);
    should_clause7->SetLeafQuery(leaf_queries);
    should_clause8->SetLeafQuery(leaf_queries);
    should_clause9->SetLeafQuery(leaf_queries);
    should_clause10->SetLeafQuery(leaf_queries);

    auto must_not_clause1 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST_NOT);
    auto must_not_clause2 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST_NOT);
    auto must_not_clause3 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST_NOT);
    auto must_not_clause4 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST_NOT);
    auto must_not_clause5 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST_NOT);
    auto must_not_clause6 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST_NOT);
    auto must_not_clause7 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST_NOT);
    auto must_not_clause8 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST_NOT);
    auto must_not_clause9 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST_NOT);
    auto must_not_clause10 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST_NOT);
    must_not_clause1->SetLeafQuery(leaf_queries);
    must_not_clause2->SetLeafQuery(leaf_queries);
    must_not_clause3->SetLeafQuery(leaf_queries);
    must_not_clause4->SetLeafQuery(leaf_queries);
    must_not_clause5->SetLeafQuery(leaf_queries);
    must_not_clause6->SetLeafQuery(leaf_queries);
    must_not_clause7->SetLeafQuery(leaf_queries);
    must_not_clause8->SetLeafQuery(leaf_queries);
    must_not_clause9->SetLeafQuery(leaf_queries);
    must_not_clause10->SetLeafQuery(leaf_queries);

    auto case1 = std::make_shared<BooleanClause<uint64_t>>();
    case1->AddBooleanClause(must_clause1);
    case1->AddBooleanClause(must_clause2);
    case1->AddBooleanClause(must_clause3);
    case1->AddBooleanClause(must_clause4);
    case1->AddBooleanClause(must_clause5);
    case1->AddBooleanClause(must_clause6);
    case1->AddBooleanClause(must_clause7);
    case1->AddBooleanClause(must_clause8);
    case1->AddBooleanClause(must_clause9);
    case1->AddBooleanClause(must_clause10);
    case1->AddBooleanClause(should_clause1);
    case1->AddBooleanClause(must_not_clause1);
//    auto res1 = proxy->Query(case1);

    auto case2 = std::make_shared<BooleanClause<uint64_t>>();
    case2->AddBooleanClause(must_clause1);
    case2->AddBooleanClause(must_clause2);
    case2->AddBooleanClause(must_clause3);
    case2->AddBooleanClause(must_clause4);
    case2->AddBooleanClause(must_clause5);
    case2->AddBooleanClause(must_clause6);
    case2->AddBooleanClause(must_clause7);
    case2->AddBooleanClause(must_clause8);
    case2->AddBooleanClause(must_clause9);
    case2->AddBooleanClause(must_clause10);

    case2->AddBooleanClause(should_clause1);
    case2->AddBooleanClause(should_clause2);
    case2->AddBooleanClause(should_clause3);
    case2->AddBooleanClause(should_clause4);
    case2->AddBooleanClause(should_clause5);
    case2->AddBooleanClause(should_clause6);
    case2->AddBooleanClause(should_clause7);
    case2->AddBooleanClause(should_clause8);
    case2->AddBooleanClause(should_clause9);
    case2->AddBooleanClause(should_clause10);

    case2->AddBooleanClause(must_not_clause1);
    case2->AddBooleanClause(must_not_clause2);
    case2->AddBooleanClause(must_not_clause3);
    case2->AddBooleanClause(must_not_clause4);
    case2->AddBooleanClause(must_not_clause5);
    case2->AddBooleanClause(must_not_clause6);
    case2->AddBooleanClause(must_not_clause7);
    case2->AddBooleanClause(must_not_clause8);
    case2->AddBooleanClause(must_not_clause9);
    case2->AddBooleanClause(must_not_clause10);
    auto res2 = proxy->Query(case2);
}

void leaf_query_case(uint64_t term_num, uint64_t range_num, uint64_t vector_num) {
    auto leaf_queries = GenSpecLeafQuery(term_num, range_num, vector_num);
    auto ms = std::make_shared<BooleanClause<uint64_t>>();
    auto must_clause1 = std::make_shared<BooleanClause<uint64_t>>(Occur::MUST);
    must_clause1->SetLeafQuery(leaf_queries);
    auto should_clause1 = std::make_shared<BooleanClause<uint64_t>>(Occur::SHOULD);
    should_clause1->SetLeafQuery(leaf_queries);
    ms->AddBooleanClause(must_clause1);
    ms->AddBooleanClause(should_clause1);
    auto res = proxy->Query(ms);
}

void
ClientTest::Test(const std::string& address, const std::string& port) {
    ConnectParam conn{address, port};
    proxy = std::make_shared<ClientProxy>();
    Status s = proxy->Connect(conn);
 

//    _1_query_case();

//    _2_query_case();

//    _3_query_case();

//    other_query_cases();

    leaf_query_case(2, 2, 2);

//    test_performance();

}
