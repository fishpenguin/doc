# Milvus DSL Design Doc

## Field datatypes

- [x] **Vector**

Record vectors of float values
```json
{
  "vector_1": {
    "type": "vector",
    "metric_type": "IP",
    "dimension": 512
  },
  "vector_2": {
    "type": "vector",
    "metric_type": "L1",
    "dimension": 256,
    "index": {
      "type": "IVF_SQ8",
      "nlist": 8792
    }
  }
}
```
>*vector_1*: `vector`, `IP`, `FLAT`, `16384`

>*vector_2*: `vector`, `L1`, `IVF_SQ8`, `8792`

| Vector Para | Required | Type      | Default        | Description                                     |
| ---------   | -------- | --------- | -------------- | ----------------------------------------------- |
| metric_type | `N` | `string`  | `L2` | Specify vector metric type. `L1`, `L2`, `IP` ... |
| dimension | `Y` | `integer`  |  | Specify vector dimension. |
| datatype | `N` | `string`  | `float` | Specify vector element data type. `float`, `integer` |

- [x] **Numeric**

`long`, `integer`, `short`, `byte`, `double`, `float`

- [x] **Boolean**

`boolean`

- [ ] **string**

`text`

- [ ] **Binary**

`binary`

- [ ] **Object**

`object` for single JSON objects

### Parameters for fields

- [x] **boost**
```json
{
  "age": {
    "type": "integer",
    "boost": 2.0
  }
}
```

| Field | Required | Type  | Default     | Description                                     |
| ---------   | --------- | -------- | -------------- | ----------------------------------------------- |
| Numeric | `N` | `float`  | `1.0` | Mapping field-level query time boosting |
| Vector | `N` | `float`  | `1.0` | Mapping field-level query time boosting |

- [x] **index**
```json
{
  "age": {
    "type": "integer",
    "index": true
  }
}
```
```json
{
  "vector_1": {
    "type": "vector",
    "metric_type": "IP",
    "dimension": 512
  },
  "vector_2": {
    "type": "vector",
    "metric_type": "L1",
    "dimension": 256,
    "index": {
      "type": "IVF_SQ8",
      "nlist": 8792
    }
  }
}
```
>*vector_1*: `vector`, `IP`, 512, `FLAT`, `16384`

>*vector_2*: `vector`, `L1`, 256, `IVF_SQ8`, `8792`

| Field | Required | Type  | Default     | Description                                     |
| ---------   | --------- | -------- | -------------- | ----------------------------------------------- |
| Numeric | `N` | `boolean`  | `true` | Should the field be indexed |
| Vector | `N` | `JSON`  | `{}` | Specify the vector index config |

| Vector Index | Required | Type      | Default        | Description                                     |
| ---------   | -------- | --------- | -------------- | ----------------------------------------------- |
| type | `Y` | `string`  | `FLAT` | Specify index type. `FLAT`, `IVF_FLAT`, `IVF_SQ8` ... |
| nlist | `N` | `integer`  | 16384 | Specify ivf nlist. |

- [ ] **null_value**

Accepts a numeric value of the samve `type` as the field which is substituted for any explicit `null` valus. Defaults to `null`

- [ ] **store**

Whether the field value should be stored and retrivable separately from the `_source` field. Accepts `true` or `false`(default).

## Mapping

### DSL
#### Create
```js
PUT /my-index
```
```json
{
  "mappings": {
    "properties": {
      "face_img_vec": {
        "type": "vector",
        "dimension": 4,
        "datatype": "float",
        "metric_type": "L2",
        "index": {
            "type": "ivf_sq8",
            "nlist": 8792
        },
        "boost": 1.5
      },
      "body_img_vec": {
        "type": "vector",
        "dimension": 8,
        "datatype": "float"
      },
      "age": {
        "type": "integer",
        "boost": 2.0,
        "index": true
      }
    }
  }
}
```
#### Get
```js
GET /my-index/_mapping
```
The API returns the following response:
```json
{
    "my-index": {
      "mappings": {
        "properties": {
          "face_img_vec": {
            "type": "vector",
            "dimension": 4,
            "metric_type": "L2",
            "datatype": "float",
            "index": {
                "type": "ivf_sq8",
                "nlist": 8792
            },
            "boost": 1.5
          },
          "body_img_vec": {
            "type": "vector",
            "dimension": 4,
            "datatype": "float"
          },
          "age": {
            "type": "integer",
            "boost": 2.0,
            "index": true
          }
        }
      },
      "settings": {
        "index": {
            "creation_date": "1576134581399",
            "provided_name": "my-index",
            "uuid": "VfR0x-sIT8WP-FL6XU9kRw"
        }
      }
    }
}
```
#### Delete
```js
DELETE /my-index
DELETE /my-index1,my-index2
DELETE /_all
```
Pass Responses
```json
{
    "acknowledged": true
}
```
Fail Responses
```json
{
    "error": {
        "index": "my-index",
        "index_uuid": "_na_",
        "reason": "no such index [my-index]",
        "root_cause": [
            {
                "index": "my-index",
                "index_uuid": "_na_",
                "reason": "no such index [my-index]",
                "type": "index_not_found_exception"
            }
        ],
        "type": "index_not_found_exception"
    },
    "status": 404
}
```

- [x] **Explicit mappings**

Fields and mapping types need to be defined before being used.

- [x] **Index field mappings can only be initialized once**

Index field mappings can only be initialized at creating index.

- [x] **Dynamic mapping**

Fields and mapping types do not need to be defined before being used.

- [x] **Field mappings modification**

Field mappings can be modified after first initialization.

## Document
### Add Document

#### Request URL
```js
POST /<index>/_doc

POST /<index>/_doc/<doc_id>
```
**`index`** Specify which index to be added

**`doc_id`** Specify the doc id of document to be added. If not specified, system will generate a unique `id` for this document.

>**Note**

>If doc_id is specified and there is a document with same doc_id in system, it will update the document.

#### Request Payload
```json
{
    "<field1>": "<value1>",
    "<field2>": "<value2>",
    "..." : "...",
    "<fieldN>": "<valueN>"
}
```


#### Response Contents

**Success**
```json
{
    "code": 0,
    "reason": "created",
    "result": {
        "_id": 87566639989444,
        "_index": "my-index",
        "_seq_no": 1,
        "_type": "_doc",
        "_version": 1,
        "_code": "0",
        "_reason": ""
    }
}
```

**Fail**
```json
{
    "code": "<error code>",
    "reason": "<error reason>",
    "details": [
      {
        "_id": "<id>",
        "_index": "<index>",
        "_seq_no": 1,
        "_type": "_doc",
        "_code": "<error code>",
        "_reason": "<error reason>"
      },
      "..."
    ]
}
```

| Field | Scope | Description                                     |
| --------------| ----------- | --------------------------------- |
| `_id` | `Success`, `Fail` | Doc id |
| `_index` | `Success`, `Fail` | Index name |
| `_code` | `Success`, `Fail` | Error code. 0 for Success |
| `_reason` | `Success`, `Fail` | Error reason. ` ` for Success |
| `_seq_no` | `Success`, `Fail` | Operation sequence number |
| `_type` | `Success`, `Fail` | Always `_doc` |
| `_version` | `Success` | doc version |


#### Response Status & Code & Reason

| Status | Code | Reason      | Description                                     | TODO |
| --------- | -------- | -------------- | ----------------------------------------------- | ---- |
| `201` | `0` | `Created` | New Doc Created | |
| `200` | `0` | `OK` | Doc Updated | |
| `404` | `40004` | `IndexNotFound` | Specified index not found | |
| `400` | `41000` | `InvalidFieldName` | Invalid field name found | ***Check field name in sync mode***  |
| `400` | `41001` | `InvalidFieldValue` | Invalid field value found | ***Check field value in sync mode*** |
| `400` | `41002` | `InvalidFields` | Fields allignment check error | ***Check fields in sync mode*** |


#### Example

**Create a doc with _id=87566639989444**
```js
POST /my-index/_doc/87566639989444
```
```json
{
  "face_img_vec": [0.29, 0.37, 0.87, 0.76],
  "body_img_vec": [0.29, 0.37, 0.18, 0.94, 0.87, 0.76, 0.45, 0.63],
  "age": 17
}
```
**`201 Created`**
```json
{
  "code": 0,
  "reason": "",
  "result": {
    "_id": 87566639989444,
    "_index": "my-index",
    "_seq_no": 1,
    "_type": "_doc",
    "_version": 1,
    "_code": 0,
    "_reason": ""
  }
}
```
**`404 Not Found`**
```json
{
  "code": 40004,
  "reason": "IndexNotFound",
  "result" : {
    "_id": 87566639989444,
    "_index": "my-index",
    "_seq_no": 1,
    "_type": "_doc",
    "_code": 40004,
    "_reason": "IndexNotFound"
  }
}
```

### Delete Document

#### Request URL
```js
DELETE /<index>/_doc/<doc_id>
```

#### Response Contents

**Success: 204 OK**
```json
{
    "code": 0,
    "reason": "Deleted",
    "result": {
        "_id": 87566639989444,
        "_index": "my-index",
        "_seq_no": 1,
        "_type": "_doc",
        "_version": 1,
        "_code": "0",
        "_reason": "Deleted"
    }
}
```
**Fail**
```json
{
    "code": 40004,
    "reason": "IndexNotFound",
    "result": {
        "_id": 87566639989444,
        "_index": "my-index",
        "_seq_no": 1,
        "_type": "_doc",
        "_code": "40004",
        "_reason": "IndexNotFound"
    }
}
```

#### Response Status & Code & Reason

| Status | Code | Reason      | Description                                     | TODO |
| --------- | -------- | -------------- | ----------------------------------------------- | ---- |
| `204` | `0` | `Deleted` | Doc deleted | |
| `404` | `40004` | `IndexNotFound` | Specified index not found | |
| `404` | `40004` | `DocNotFound` | Specified doc not found | ***Need check docid in sync mode*** |

## Search

| keyword | Requried | Default |  Description                                     | TODO |
| --------- | ---- | ----- | ----------------------------------------------- | ---- |
| `from` | `N` | 0 | Defines the offset from the first result to fetch | |
| `size` | `N` | 10 | Configure the maximum amount of hits to be returned | |
| `min_score` | `N` | Infinite | results below the min score cannot be returned | |
| `sort` | `N` | By Score | results below the min score cannot be returned | Support `Mode` and sort by `field` |
| `_source` | `N` | false | Specify the raw fields info in results | |



### Single Search

#### Request URL
```
POST /<index>/_search
```

#### Request Body

*Match Query*
```json
{
    "query": {
        "match" :{
            "<field>": {
                "query" : "<query value>",
                "analyzer": "<analyzer>"
            }
        }
    }
}
```
```json
{
    "query": {
        "match" : {
            "<field>": "<query value>"
        }
    }
}
```

*Numeric Range Query*
```json
{
    "query": {
        "range" : {
            "<field>" : {
                "gte" : "<min_val>",
                "lte" : "<max_val>",
                "boost" : "<max_val>"
            }
        }
    }
}
```

*Vector Field Query*

| Param | Requried | Default |  Description                                     | TODO |
| --------- | ---- | ----- | ----------------------------------------------- | ---- |
| `min_score` | `N` | Infinite | results below the min score cannot be returned | |
| `boost` | `N` | 1.0 | score boost value | |
| `score_mode` | `N` | sum | sum, avg, max, min | |
| `nprobe` | `N` | 32 | Only valid for vector field with ivf index | |
| `topk` | `N` | 10 | vector topk | |
| `nq` | `Y` | - | Specify the n query for this search | |
| `query` | `Y` | - | query vector of shape (nq * dimension) | |

```json
{
    "query": {
        "vector": {
            "<field1>": {
                "nprobe": "<nprobe value>",
                "topk": "<top k value>",
                "nq": "<n query value>",
                "query": [
                    [0.1, 0.2, "..."],
                    "..."
                ],
                "boost": "<boost value>",
                "min_score": "<min score limit>"
            },
            "<field2>": {
                "...": "..."
            },
            "score_mode": "sum",
            "boost": "<boost value>",
            "min_score": "<min score limit>"
        }
    }
}
```

*Boolean Query*

| Occur | Description                                     | TODO |
| --------- |  ----------------------------------------------- | ---- |
| `must` | The clause (query) must appear in matching documents and will contribute to the score | |
| `must_not` | The clause (query) must not appear in matching documents. Clauses are executed in filter context meaning that scoring is ignored and clauses are considered for caching. Because scoring is ignored, a score of 0 for all documents is returned. | |
| `should` | The clause (query) should appear in the matching document. | |
| `filter` | The clause (query) must appear in matching documents. However unlike must the score of the query will be ignored. Filter clauses are executed in filter context, meaning that scoring is ignored and clauses are considered for caching. | |

```json
{
  "query": {
    "bool" : {
      "must" : {
        "match" : {
            "<field1>" : {
                "query": "<query value>"
            }
        }
      },
      "filter": {
        "match" : {
            "<field2>" : {
                "query": "<query value>"
            }
        }
      },
      "must_not" : {
        "range" : {
          "<field3>" : { "gte" : "<min value>", "lte" : "<max value>" }
        }
      },
      "should" : [
        { "match" : { "<field4>" : "<query value>" } },
        { "match" : { "<field5>" : "<query value>" } }
      ],
      "minimum_should_match" : 1,
      "boost" : 1.0
    }
  },
  "min_score": "<min score limit>",
  "sort": ["<field1>", "<field2>"],
  "from": "<offset from first>",
  "size": "<fetch size>"
}
```

*Source Filter*
```json
{
    "_source": false
}
```
```json
{
    "_source": ["<field1>", "<field2>"]
}
```
```json
{
    "_source": {
        "exclude": ["<field1>"]
    }
}
```

*Score Mode*
```json
{
    "query": {
        "bool": {
            "must": {
                "match" : {"<field1>": "<query value>"},
                "match" : {"<field2>": "<query value>"},
                "score_mode": "sum",
                "boost": 2.0
            }
        }

    }
}
```

#### Example
```json
{
    "query": {
        "bool": {
            "should": {
                "range": {
                    "age": {"gte": 18, "lte": 35},
                    "boost": 5.0
                },
                "range": {
                    "age": {"gte": 36, "lte": 60},
                    "boost": 2.0
                },
                "vector": {
                    "head": {
                        "nprobe": 64,
                        "topk": 100,
                        "nq": 2,
                        "query": [
                            [0.1, 0.2, "..."],
                            "..."
                        ],
                        "boost": 5.0,
                        "min_score": 2.0
                    }

                },
                "minimum_should_match": 1,
                "boost": 3.0,
                "score_mode": "sum"
            },
            "filter": {
                "match": {
                    "sex": "female"
                }
            }
        }
    }
}
```

#### Response
