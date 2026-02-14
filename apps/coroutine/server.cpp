#include <boost/asio.hpp>
#include <iostream>

using boost::asio::awaitable;
using boost::asio::detached;
using boost::asio::ip::tcp;

awaitable<void> session(tcp::socket socket)
{
    std::array<char, 1024> data {};
    std::size_t            n = co_await socket.async_read_some(
        boost::asio::buffer(data),
        boost::asio::use_awaitable);
    std::string out = "echo(coro): " + std::string(data.data(), n);
    co_await boost::asio::async_write(
        socket,
        boost::asio::buffer(out),
        boost::asio::use_awaitable);
}

awaitable<void> listener()
{
    auto          ex = co_await boost::asio::this_coro::executor;
    tcp::acceptor acceptor(ex, {tcp::v4(), 9301});
    std::cout << "coroutine server on :9301\n";
    while (true)
    {
        tcp::socket socket
            = co_await acceptor.async_accept(boost::asio::use_awaitable);
        boost::asio::co_spawn(ex, session(std::move(socket)), detached);
    }
}

int main()
{
    boost::asio::io_context io;
    boost::asio::co_spawn(io, listener(), detached);
    io.run();
}
