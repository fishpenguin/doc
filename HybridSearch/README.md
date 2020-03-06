# Notes
## Added properties
* Field
```
Field is defined in Field.h
Fieled include struct field and vectorfield. we use struct field as the definition of numerica
field. Field will be used in CollecionSchema
```
* Query
```
BooleanQuery defined in BooleanQuery.h, other queries defined in GeneralQuery.h
These queries will be used in Search interface. Users need to construct a BooleanQuery object,
then call search function.
```

## Changed interface
* About Collection
```
CollectionSchema: include numerica fields and vector fields
```

* About Document
```
Insert: replaced by InsertDocument
GetVectorByID: replaced by GetDocByID
DeleteByID: delete documents by doc ids
Search: use a BooleanQuery object as parameter

```
