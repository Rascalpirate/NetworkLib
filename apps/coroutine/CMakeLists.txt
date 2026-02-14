add_executable(coro_server server.cpp)
add_executable(coro_client client.cpp)

target_link_libraries(coro_server PRIVATE Boost::system pthread)
target_link_libraries(coro_client PRIVATE Boost::system pthread)
