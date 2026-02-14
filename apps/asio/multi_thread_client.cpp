#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <vector>

using boost::asio::ip::tcp;

static void one_client(int i)
{
    boost::asio::io_context io;
    tcp::socket             socket(io);
    socket.connect({boost::asio::ip::make_address("127.0.0.1"), 9102});
    std::string msg = "hello asio mt client " + std::to_string(i);
    boost::asio::write(socket, boost::asio::buffer(msg));
    std::array<char, 1024> data {};
    std::size_t            n = socket.read_some(boost::asio::buffer(data));
    std::cout << std::string(data.data(), n) << "\n";
}

int main()
{
    std::vector<std::thread> ts;
    for (int i = 0; i < 4; ++i)
        ts.emplace_back(one_client, i);
    for (auto &t : ts)
        t.join();
}
