# Milvus DSL Design Doc

## Field datatypes

- [x] **Vector** Record vectors of float values

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
```js
PUT /my-index
```
```json
{
  "mappings": {
    "properties": {
      "face_img_vec": {
        "type": "vector",
        "dimension": 512,
        "metric_type": "L2",
        "index": {
            "type": "ivf_sq8",
            "nlist": 8792
        },
        "boost": 1.5
      },
      "body_img_vec": {
        "type": "vector",
        "dimension": 256
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
- [x] **Explicit mappings**

Fields and mapping types need to be defined before being used.

- [x] **Index field mappings can only be initialized once**

Index field mappings can only be initialized at creating index.

- [x] **Dynamic mapping**

Fields and mapping types do not need to be defined before being used.

- [x] **Field mappings modification**

Field mappings can be modified after first initialization.
