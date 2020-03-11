

add_executable (milvuspb_server
    main.cpp
    GrpcServer.cpp
)

install(TARGETS milvuspb_server DESTINATION bin)