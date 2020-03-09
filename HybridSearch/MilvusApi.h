#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Status.h"
#include "BooleanQuery.h"

/** \brief Milvus SDK namespace
 */
namespace milvus {
/**
 * @brief Index Type
 */
enum class IndexType {
    INVALID = 0,
    FLAT = 1,
    IVFFLAT = 2,
    IVFSQ8 = 3,
    RNSG = 4,
    IVFSQ8H = 5,
    IVFPQ = 6,
    SPTAGKDT = 7,
    SPTAGBKT = 8,
    HNSW = 11,
};

/**
 * @brief Connect API parameter
 */
struct ConnectParam {
    std::string ip_address;  ///< Server IP address
    std::string port;        ///< Server PORT
};

/**
 * @brief Field Type ?????????????????????????????????????????????????????????
 */
struct FieldType {
    Field numeric_field;
    VectorField vector_field;
}

/**
 * @brief Field Mapping
 */
struct FieldMapping {
    uint64_t field_id;
    string field_name;
    FieldType field_type;     ///< vector or numerica(int64_t, float...)
}

/**
 * @brief Collection Mapping
 */
struct Mapping {
    uint64_t collection_id;                        ///< Collection id
    std::string collection_name;                   ///< Collection name
    std::vector<FieldMapping> fields;        ///< Fields
};

/**
 * @brief Record inserted
 */
struct RowRecord {
    std::vector<float> float_data;     ///< Vector raw float data
    std::vector<uint8_t> binary_data;  ///< Vector raw binary data
};

/**
 * @brief Entity Record inserted
 */
struct EntityRecord {
    uint64_t entity_id;
    // list all kinds fields?
    // Or use string, user can input whatever kind of value they want,
    // and the value will be describe as a json string
    std::map<std::string, RowRecord> vector_field_value;
    std::map<std::string, int64_t> int64_value;
    std::map<std::string, int32_t> int32_value;
    std::map<std::string, short>   short_value;
    std::map<std::string, float>   float_value;
    std::map<std::string, double>  double_value;
    std::map<std::string, std::string> string_value;
}

/**
 * @brief Query result
 */
struct QueryResult {
    EntityRecord entity_record;
    float score;
    float distance;
};
using TopkQueryResult = std::vector<QueryResult>;

/**
 * @brief Index parameters
 * Note: extra_params is extra parameters list, it must be json format
 *       For different index type, parameter list is different accordingly, for example:
 *       FLAT/IVFLAT/SQ8:  "{nlist: '16384'}"
 *       IVFPQ:  "{nlist: '16384', nbits: "16"}"
 *       NSG:  "{search_length: '100', out_degree:'40', pool_size:'66'}"
 *       HNSW  "{M: '16', ef_construct:'500'}"
 */
struct IndexParam {
    std::string collection_name;             ///< Collection name for create index
    IndexType index_type;               ///< Index type
    std::string extra_params;           ///< Extra parameters according to different index type, must be json format
};

/**
 * @brief partition parameters
 */
struct PartitionParam {
    std::string collection_name;
    std::string partition_tag;
};

using PartitionTagList = std::vector<std::string>;

/**
 * @brief segment statistics
 */
struct SegmentStat {
    std::string segment_name;    ///< Segment name
    int64_t row_count;           ///< Segment row count
    std::string index_name;      ///< Segment index name
    int64_t data_size;           ///< Segment data size
};

/**
 * @brief partition statistics
 */
struct PartitionStat {
    std::string tag;                          ///< Partition tag
    int64_t row_count;                        ///< Partition row count
    std::vector<SegmentStat> segments_stat;   ///< Partition's segments statistics
};

/**
 * @brief collection info
 */
struct CollectionInfo {
    int64_t total_row_count;                  ///< Collection total row count
    std::vector<PartitionStat> partitions_stat;   ///< Collection's partitions statistics
};

/**
 * @brief SDK main class
 */
class Connection {
 public:
    /**
     * @brief CreateConnection
     *
     * Create a connection instance and return it's shared pointer
     *
     * @return Connection instance pointer
     */

    static std::shared_ptr<Connection>
    Create();

    /**
     * @brief DestroyConnection
     *
     * Destroy the connection instance
     *
     * @param connection, the shared pointer to the instance to be destroyed
     *
     * @return if destroy is successful
     */

    static Status
    Destroy(std::shared_ptr<Connection>& connection_ptr);

    /**
     * @brief Connect
     *
     * This method is used to connect server.
     * Connect function should be called before any operations.
     *
     * @param param, use to provide server information
     *
     * @return Indicate if connect is successful
     */

    virtual Status
    Connect(const ConnectParam& param) = 0;

    /**
     * @brief Connect
     *
     * This method is used to connect server.
     * Connect function should be called before any operations.
     *
     * @param uri, use to provide server uri, example: milvus://ipaddress:port
     *
     * @return Indicate if connect is successful
     */
    virtual Status
    Connect(const std::string& uri) = 0;

    /**
     * @brief connected
     *
     * This method is used to test whether server is connected.
     *
     * @return Indicate if connection status
     */
    virtual Status
    Connected() const = 0;

    /**
     * @brief Disconnect
     *
     * This method is used to disconnect server.
     *
     * @return Indicate if disconnect is successful
     */
    virtual Status
    Disconnect() = 0;

    /**
     * @brief Create collection method
     *
     * This method is used to create collection.
     *
     * @param param, use to provide collection information to be created.
     *
     * @return Indicate if collection is created successfully
     */
    virtual Status
    CreateCollection(const Mapping& param) = 0;

    /**
     * @brief Test collection existence method
     *
     * This method is used to create collection.
     *
     * @param collection_name, target collection's name.
     *
     * @return Indicate if collection is cexist
     */
    virtual bool
    HasCollection(const std::string& collection_name) = 0;

    /**
     * @brief Drop collection method
     *
     * This method is used to drop collection(and its partitions).
     *
     * @param collection_name, target collection's name.
     *
     * @return Indicate if collection is drop successfully.
     */
    virtual Status
    DropCollection(const std::string& collection_name) = 0;

    /**
     * @brief Create index method
     *
     * This method is used to create index for whole collection(and its partitions).
     *
     * @param IndexParam
     *  collection_name, collection name is going to be create index.
     *  index type,
     *  nlist,
     *  index file size
     *
     * @return Indicate if build index successfully.
     */
    virtual Status
    CreateIndex(const IndexParam& index_param) = 0;

    /**
     * @brief Insert vector to collection
     *
     * This method is used to insert vector array to collection.
     *
     * @param collection_name, target collection's name.
     * @param partition_tag, target partition's tag, keep empty if no partition.
     * @param record_array, vector array is inserted.
     * @param id_array,
     *  specify id for each vector,
     *  if this array is empty, milvus will generate unique id for each vector,
     *  and return all ids by this parameter.
     *
     * @return Indicate if vector array are inserted successfully
     */
    virtual Status
    Insert(const std::string& collection_name, const std::string& partition_tag, const std::vector<RowRecord>& record_array,
           std::vector<int64_t>& id_array) = 0;

    // Insert replaced by InsertEntity
    virtual Status
    InsertEntity(const std::string& collection_name, const std::string& partition_tag, std::vector<EntityRecord> entity_records,
            std::vector<uint64_t>& id_array) = 0;

    /**
     * @brief Get vector data by id
     *
     * This method is used to get vector data by id from a collection.
     * Return the first found vector if there are vectors with duplicated id
     *
     * @param collection_name, target collection's name.
     * @param vector_id, target vector id.
     * @param vector_data, returned vector data.
     *
     * @return Indicate if the operation is succeed.
     */
    virtual Status
    GetVectorByID(const std::string& collection_name, int64_t vector_id, RowRecord& vector_data) = 0;

    virtual Status
    GetEntityByID(const std::string& collection_name, int64_t entity_id, EntityRecord& entity_record) = 0;

    /**
     * @brief Get vector ids from a segment
     *
     * This method is used to get vector ids from a segment
     * Return all vector(not deleted) ids
     *
     * @param collection_name, target collection's name.
     * @param segment_name, target segment name.
     * @param id_array, returned vector id array.
     *
     * @return Indicate if the operation is succeed.
     */
    virtual Status
    GetIDsInSegment(const std::string& collection_name, const std::string& segment_name, std::vector<int64_t>& id_array) = 0;

    /**
     * @brief Search vector
     *
     * This method is used to query vector in collection.
     *
     * @param collection_name, target collection's name.
     * @param partition_tags, target partitions, keep empty if no partition.
     * @param query_record_array, vectors to be queried.
     * @param topk, how many similarity vectors will be searched.
     * @param nprobe, the number of centroids choose to search.
     * @param topk_query_result_array, result array.
     *
     * @return Indicate if query is successful.
     */
    virtual Status
    Search(const std::string& collection_name, const std::vector<std::string>& partition_tags,
           const std::vector<RowRecord>& query_record_array, int64_t topk,
           int64_t nprobe, TopKQueryResult& topk_query_result) = 0;
    
    virtual Status
    Search(const std::string& collection_name, const std::vector<std::string>& partition_tags,
            const std::vector<BooleanQueryPtr> boolean_queries, TopkQueryResult& topk_query_result) = 0;

    /**
     * @brief Show collection description
     *
     * This method is used to show collection information.
     *
     * @param collection_name, target collection's name.
     * @param collection_mapping, collection_mapping is given when operation is successful.
     *
     * @return Indicate if this operation is successful.
     */
    virtual Status
    DescribeCollection(const std::string& collection_name, Mapping& collection_mapping) = 0;

    /**
     * @brief Get collection row count
     *
     * This method is used to get collection row count.
     *
     * @param collection_name, target collection's name.
     * @param row_count, collection total row count(including partitions).
     *
     * @return Indicate if this operation is successful.
     */
    virtual Status
    CountCollection(const std::string& collection_name, int64_t& row_count) = 0;

    /**
     * @brief Show all collections in database
     *
     * This method is used to list all collections.
     *
     * @param collection_array, all collections in database.
     *
     * @return Indicate if this operation is successful.
     */
    virtual Status
    ShowCollections(std::vector<std::string>& collection_array) = 0;

    /**
     * @brief Show collection information
     *
     * This method is used to get detail information of a collection.
     *
     * @param collection_name, target collection's name.
     * @param collection_info, target collection's information
     *
     * @return Indicate if this operation is successful.
     */
    virtual Status
    ShowCollectionInfo(const std::string& collection_name, CollectionInfo& collection_info) = 0;

    /**
     * @brief Give the client version
     *
     * This method is used to give the client version.
     *
     * @return Client version.
     */
    virtual std::string
    ClientVersion() const = 0;

    /**
     * @brief Give the server version
     *
     * This method is used to give the server version.
     *
     * @return Server version.
     */
    virtual std::string
    ServerVersion() const = 0;

    /**
     * @brief Give the server status
     *
     * This method is used to give the server status.
     *
     * @return Server status.
     */
    virtual std::string
    ServerStatus() const = 0;

    /**
     * @brief dump server tasks information
     *
     * This method is internal used.
     *
     * @return Task information in taskcollections.
     */
    virtual std::string
    DumpTaskTabless() const = 0;

    /**
     * [deprecated]
     * @brief delete collections by vector id
     *
     * This method is used to delete collection data by date range.
     *
     * @param collection_name, target collection's name.
     * @param id_array, vector ids to deleted.
     *
     * @return Indicate if this operation is successful.
     */
    virtual Status
    DeleteByID(const std::string& collection_name, const std::vector<int64_t>& id_array) = 0;
    // Delete entitys by entity ids

    /**
     * @brief preload collection
     *
     * This method is used to preload collection
     *
     * @param collection_name
     *
     * @return Indicate if this operation is successful.
     */
    virtual Status
    PreloadCollection(const std::string& collection_name) const = 0;

    /**
     * @brief describe index
     *
     * This method is used to describe index
     *
     * @param collection_name, target collection's name.
     * @param index_param, returned index information.
     *
     * @return Indicate if this operation is successful.
     */
    virtual Status
    DescribeIndex(const std::string& collection_name, IndexParam& index_param) const = 0;

    virtual Status
    DescribeIndex(const std::string& collection_name, std::vector<std::string>& field_name_array, IndexParam& index_param) const = 0;

    /**
     * @brief drop index
     *
     * This method is used to drop index of collection(and its partitions)
     *
     * @param collection_name, target collection's name.
     *
     * @return Indicate if this operation is successful.
     */
    virtual Status
    DropIndex(const std::string& collection_name) const = 0;

    virtual Status
    DropIndex(const std::string & collection_name, std::vector<std::string>& field_name_array) const = 0;

    /**
     * @brief Create partition method
     *
     * This method is used to create collection partition
     *
     * @param param, use to provide partition information to be created.
     *
     * @return Indicate if partition is created successfully
     */
    virtual Status
    CreatePartition(const PartitionParam& param) = 0;

    /**
     * @brief Test collection existence method
     *
     * This method is used to create collection
     *
     * @param collection_name, collection name is going to be tested.
     * @param partition_array, partition tag array of the collection.
     *
     * @return Indicate if this operation is successful
     */
    virtual Status
    ShowPartitions(const std::string& collection_name, PartitionTagList& partition_array) const = 0;

    /**
     * @brief Delete partition method
     *
     * This method is used to delete collection partition.
     *
     * @param param, target partition to be deleted.
     *      NOTE: if param.collection_name is empty, you must specify param.partition_name,
     *          else you can specify param.collection_name and param.tag and let the param.partition_name be empty
     *
     * @return Indicate if partition is delete successfully.
     */
    virtual Status
    DropPartition(const PartitionParam& param) = 0;

    /**
     * @brief Get config method
     *
     * This method is used to set config.
     *
     * @param node_name, config node name.
     * @param value, config value.
     *
     * @return Indicate if this operation is successful.
     */
    virtual Status
    GetConfig(const std::string& node_name, std::string& value) const = 0;

    /**
     * @brief Set config method
     *
     * This method is used to set config.
     *
     * @param node_name, config node name.
     * @param value, config value.
     *
     * @return Indicate if this operation is successful.
     */
    virtual Status
    SetConfig(const std::string& node_name, const std::string& value) const = 0;

    /**
     * @brief flush collection buffer into storage
     *
     * This method is used to flush collection buffer into storage
     *
     * @param collection_name, target collection's name.
     *
     * @return Indicate if this operation is successful.
     */
    virtual Status
    FlushCollection(const std::string& collection_name) = 0;

    /**
     * @brief flush all buffer into storage
     *
     * This method is used to all collection buffer into storage
     *
     * @return Indicate if this operation is successful.
     */
    virtual Status
    Flush() = 0;

    /**
     * @brief compact collection, remove deleted vectors
     *
     * This method is used to compact collection
     *
     * @param collection_name, target collection's name.
     *
     * @return Indicate if this operation is successful.
     */
    virtual Status
    CompactCollection(const std::string& collection_name) = 0;
};

}  // namespace milvus