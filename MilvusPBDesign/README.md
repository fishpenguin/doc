# Notes
## Generate GRPC files
* Generate Protobuf Message Files
```bash
>>> protoc -I /usr/local/include/google/protobuf --cpp_out=. --proto_path . common.proto
```

* Generate GRPC Service Files
```bash
>>> protoc -I . --grpc_out=. --plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin common.proto
```
