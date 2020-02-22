add_executable(sdk_simple
        main.cpp
        ClientTest.cpp
        ClientTest.h
        )

target_link_libraries(sdk_simple
        milvuspb_sdk
        pthread
        )

install(TARGETS sdk_simple DESTINATION bin)