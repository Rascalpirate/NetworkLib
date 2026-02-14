set(PROTO_FILE ${CMAKE_CURRENT_SOURCE_DIR}/chat.proto)
get_filename_component(PROTO_DIR ${PROTO_FILE} DIRECTORY)

set(PROTO_SRCS ${CMAKE_CURRENT_BINARY_DIR}/chat.pb.cc)
set(PROTO_HDRS ${CMAKE_CURRENT_BINARY_DIR}/chat.pb.h)
set(GRPC_SRCS ${CMAKE_CURRENT_BINARY_DIR}/chat.grpc.pb.cc)
set(GRPC_HDRS ${CMAKE_CURRENT_BINARY_DIR}/chat.grpc.pb.h)

add_custom_command(
  OUTPUT ${PROTO_SRCS} ${PROTO_HDRS} ${GRPC_SRCS} ${GRPC_HDRS}
  COMMAND protobuf::protoc
  ARGS --grpc_out ${CMAKE_CURRENT_BINARY_DIR}
       --cpp_out ${CMAKE_CURRENT_BINARY_DIR}
       -I ${PROTO_DIR}
       --plugin=protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
       ${PROTO_FILE}
  DEPENDS ${PROTO_FILE})

add_library(grpc_chat_proto ${PROTO_SRCS} ${GRPC_SRCS})
target_include_directories(grpc_chat_proto PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(grpc_chat_proto PUBLIC gRPC::grpc++ protobuf::libprotobuf)

add_executable(grpc_modes_server server.cpp)
add_executable(grpc_modes_client client.cpp)

target_include_directories(grpc_modes_server PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(grpc_modes_client PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(grpc_modes_server PRIVATE grpc_chat_proto)
target_link_libraries(grpc_modes_client PRIVATE grpc_chat_proto)
