#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

int main()
{
    boost::asio::io_context io;
    tcp::socket             socket(io);
    socket.connect({boost::asio::ip::make_address("127.0.0.1"), 9103});
    std::string msg = "hello asio thread pool";
    boost::asio::write(socket, boost::asio::buffer(msg));
    std::array<char, 1024> data {};
    std::size_t            n = socket.read_some(boost::asio::buffer(data));
    std::cout << std::string(data.data(), n) << "\n";
}
