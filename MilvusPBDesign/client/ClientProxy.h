#include "CommonApi.h"

#include <memory>
#include <string>
#include <vector>

class ClientProxy : public Connection {
 public:
    Status
    Connect(const std::string& uri);

    Status
    Connect(const ConnectParam& param)

    Status
    CreateCollection(uint64_t id, CollectionSchema collection_schema);

    QueryResponse
    Query(BooleanClausePtr boolean_clause);

 private:
    std::shared_ptr<::demo::Channel> channel_;
    std::shared_ptr<GrpcClient> client_ptr_;
    bool connected_ = false;
}