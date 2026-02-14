#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

int main()
{
    boost::asio::thread_pool pool(4);
    boost::asio::io_context  io;
    tcp::acceptor            acceptor(io, tcp::endpoint(tcp::v4(), 9103));

    std::cout << "asio thread-pool server on :9103\n";
    while (true)
    {
        tcp::socket socket(io);
        acceptor.accept(socket);
        auto                   msg = std::make_shared<std::string>();
        std::array<char, 1024> data {};
        std::size_t            n = socket.read_some(boost::asio::buffer(data));
        *msg                     = std::string(data.data(), n);
        boost::asio::post(
            pool,
            [sock = std::move(socket), msg]() mutable
            {
                std::string out = "echo(asio-pool): " + *msg;
                boost::asio::write(sock, boost::asio::buffer(out));
            });
    }
}
