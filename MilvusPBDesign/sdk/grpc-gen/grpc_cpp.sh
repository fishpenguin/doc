#!/bin/bash
~/test/milvus/core/cmake-build-debug/grpc_ep-prefix/src/grpc_ep/bins/opt/protobuf/protoc -I . --grpc_out=./gen-status --plugin=protoc-gen-grpc=/home/yukun/test/milvus/core/cmake-build-debug/grpc_ep-prefix/src/grpc_ep/bins/opt/grpc_cpp_plugin status.proto

~/test/milvus/core/cmake-build-debug/grpc_ep-prefix/src/grpc_ep/bins/opt/protobuf/protoc -I . --cpp_out=./gen-status status.proto

~/test/milvus/core/cmake-build-debug/grpc_ep-prefix/src/grpc_ep/bins/opt/protobuf/protoc -I . --grpc_out=./gen-common --plugin=protoc-gen-grpc=/home/yukun/test/milvus/core/cmake-build-debug/grpc_ep-prefix/src/grpc_ep/bins/opt/grpc_cpp_plugin common.proto

~/test/milvus/core/cmake-build-debug/grpc_ep-prefix/src/grpc_ep/bins/opt/protobuf/protoc -I . --cpp_out=./gen-common common.proto