# Milvus DSL Design Doc

## Field datatypes

- [x] **Vector** Record vectors of float values

```json
{
  "vector_1": {
    "type": "vector",
    "metric_type": "IP"
  },
  "vector_2": {
    "type": "vector",
    "metric_type": "L1",
    "index": {
      "type": "IVF_SQ8",
      "nlist": 8792
    }
  }
}
```
>*`vector_1`*: `vector`, `IP`, `FLAT`, `16384`

>*`vector_2`*: `vector`, `L1`, `IVF_SQ8`, `8792`

| Vector Para | Required | Type      | Default        | Description                                     |
| ---------   | -------- | --------- | -------------- | ----------------------------------------------- |
| metric_type | `N` | `string`  | `L2` | Specify vector metric type. `L1`, `L2`, `IP` ... |
| dimension | `Y` | `integer`  |  | Specify vector dimension. |
| index | `N` | `JSON`  | `{}` | Specify vector index config. |

| Index Para | Required | Type      | Default        | Description                                     |
| ---------   | -------- | --------- | -------------- | ----------------------------------------------- |
| type | `Y` | `string`  | `FLAT` | Specify index type. `FLAT`, `IVF_FLAT`, `IVF_SQ8` ... |
| nlist | `N` | `integer`  | 16384 | Specify ivf nlist. |

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

All fields. Mapping field-level query time boosting. Accepts a floating point number, defaults to 1.0.

- [x] **index**

All fields. Should the field be searchable? Accepts `true`(default) and `false` for numeric fields or `JSON` body `{}`(default) | `{xxx}` for `vector` field.

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
