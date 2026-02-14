include(FetchContent)

set(ASIO_GRPC_PROTO ${CMAKE_CURRENT_SOURCE_DIR}/echo.proto)
set(ASIO_GRPC_GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${ASIO_GRPC_GEN_DIR})

set(ASIO_GRPC_PROTO_SRCS ${ASIO_GRPC_GEN_DIR}/echo.pb.cc)
set(ASIO_GRPC_PROTO_HDRS ${ASIO_GRPC_GEN_DIR}/echo.pb.h)
set(ASIO_GRPC_GRPC_SRCS ${ASIO_GRPC_GEN_DIR}/echo.grpc.pb.cc)
set(ASIO_GRPC_GRPC_HDRS ${ASIO_GRPC_GEN_DIR}/echo.grpc.pb.h)

add_custom_command(
  OUTPUT ${ASIO_GRPC_PROTO_SRCS} ${ASIO_GRPC_PROTO_HDRS} ${ASIO_GRPC_GRPC_SRCS} ${ASIO_GRPC_GRPC_HDRS}
  COMMAND protobuf::protoc
  ARGS --grpc_out ${ASIO_GRPC_GEN_DIR}
       --cpp_out ${ASIO_GRPC_GEN_DIR}
       -I ${CMAKE_CURRENT_SOURCE_DIR}
       --plugin=protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
       ${ASIO_GRPC_PROTO}
  DEPENDS ${ASIO_GRPC_PROTO}
  VERBATIM)

add_library(asio_grpc_proto ${ASIO_GRPC_PROTO_SRCS} ${ASIO_GRPC_GRPC_SRCS})
target_include_directories(asio_grpc_proto PUBLIC ${ASIO_GRPC_GEN_DIR})
target_link_libraries(asio_grpc_proto PUBLIC protobuf::libprotobuf gRPC::grpc++)

FetchContent_Declare(
  asio_grpc
  GIT_REPOSITORY https://github.com/Tradias/asio-grpc.git
  GIT_TAG v3.2.0)
FetchContent_MakeAvailable(asio_grpc)

add_executable(asio_grpc_server server.cpp)
add_executable(asio_grpc_client client.cpp)

target_include_directories(asio_grpc_server PRIVATE ${ASIO_GRPC_GEN_DIR})
target_include_directories(asio_grpc_client PRIVATE ${ASIO_GRPC_GEN_DIR})

target_link_libraries(asio_grpc_server
  PRIVATE
    asio_grpc::asio_grpc
    asio_grpc_proto
    Boost::system
    gRPC::grpc++)

target_link_libraries(asio_grpc_client
  PRIVATE
    asio_grpc::asio_grpc
    asio_grpc_proto
    Boost::system
    gRPC::grpc++)
