#!/bin/bash
~/test/milvus/core/cmake-build-debug/grpc_ep-prefix/src/grpc_ep/bins/opt/protobuf/protoc -I . --grpc_out=./grpc-gen --plugin=protoc-gen-grpc=/home/yukun/test/milvus/core/cmake-build-debug/grpc_ep-prefix/src/grpc_ep/bins/opt/grpc_cpp_plugin status.proto

~/test/milvus/core/cmake-build-debug/grpc_ep-prefix/src/grpc_ep/bins/opt/protobuf/protoc -I . --cpp_out=./grpc-gen status.proto

~/test/milvus/core/cmake-build-debug/grpc_ep-prefix/src/grpc_ep/bins/opt/protobuf/protoc -I . --grpc_out=./grpc-gen --plugin=protoc-gen-grpc=/home/yukun/test/milvus/core/cmake-build-debug/grpc_ep-prefix/src/grpc_ep/bins/opt/grpc_cpp_plugin milvus.proto

~/test/milvus/core/cmake-build-debug/grpc_ep-prefix/src/grpc_ep/bins/opt/protobuf/protoc -I . --cpp_out=./grpc-gen milvus.proto