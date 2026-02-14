#include <boost/asio.hpp>
#include <iostream>

using boost::asio::awaitable;
using boost::asio::ip::tcp;

awaitable<void> run_client()
{
    auto        ex = co_await boost::asio::this_coro::executor;
    tcp::socket socket(ex);
    co_await socket.async_connect(
        {boost::asio::ip::make_address("127.0.0.1"), 9301},
        boost::asio::use_awaitable);
    std::string msg = "hello coroutine";
    co_await boost::asio::async_write(
        socket,
        boost::asio::buffer(msg),
        boost::asio::use_awaitable);
    std::array<char, 1024> data {};
    std::size_t            n = co_await socket.async_read_some(
        boost::asio::buffer(data),
        boost::asio::use_awaitable);
    std::cout << std::string(data.data(), n) << "\n";
}

int main()
{
    boost::asio::io_context io;
    boost::asio::co_spawn(io, run_client(), [](std::exception_ptr, auto) {});
    io.run();
}
