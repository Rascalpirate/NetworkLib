#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

int main()
{
    boost::asio::io_context io;
    tcp::socket             socket(io);
    socket.connect({boost::asio::ip::make_address("127.0.0.1"), 9101});
    std::string msg = "hello asio single thread";
    boost::asio::write(socket, boost::asio::buffer(msg));
    char        data[1024] {};
    std::size_t n = socket.read_some(boost::asio::buffer(data));
    std::cout << std::string(data, n) << "\n";
}
