# Milvus DSL Design Doc

## Field datatypes

### Supported

> **Vector**

`vector` Record vectors of float values

> **Numeric**

`long`, `integer`, `short`, `byte`, `double`, `float`

> **Boolean**

`boolean`

### TODO

> **string**

`text`

> **Binary**

`binary`

> **Object**

`object` for single JSON objects

## Mapping

### Supported

> **Explicit mappings**

Fields and mapping types need to be defined before being used.

> **Index field mappings can only be initialized once**

Index field mappings can only be initialized at creating index.

### TODO

> **Dynamic mapping**

Fields and mapping types do not need to be defined before being used.

> **Field mappings modification**

Field mappings can be modified after first initialization.


```
```
