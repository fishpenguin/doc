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
