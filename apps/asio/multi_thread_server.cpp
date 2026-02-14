#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <vector>

using boost::asio::ip::tcp;

int main()
{
    boost::asio::io_context io;
    tcp::acceptor           acceptor(io, tcp::endpoint(tcp::v4(), 9102));

    std::function<void()> do_accept;
    do_accept = [&]
    {
        auto socket = std::make_shared<tcp::socket>(io);
        acceptor.async_accept(
            *socket,
            [&, socket](boost::system::error_code ec)
            {
                if (!ec)
                {
                    auto data = std::make_shared<std::array<char, 1024>>();
                    socket->async_read_some(
                        boost::asio::buffer(*data),
                        [socket,
                         data](boost::system::error_code e, std::size_t n)
                        {
                            if (!e)
                            {
                                std::string out = "echo(asio-mt): "
                                                + std::string(data->data(), n);
                                boost::asio::async_write(
                                    *socket,
                                    boost::asio::buffer(out),
                                    [socket](auto, auto) {});
                            }
                        });
                }
                do_accept();
            });
    };

    do_accept();
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i)
        threads.emplace_back(
            [&]
            {
                io.run();
            });
    std::cout << "asio multi-thread server on :9102\n";
    for (auto &t : threads)
        t.join();
}
