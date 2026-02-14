#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

int main()
{
    boost::asio::io_context io;
    tcp::acceptor           acceptor(io, tcp::endpoint(tcp::v4(), 9101));
    std::cout << "asio single-thread server on :9101\n";
    while (true)
    {
        tcp::socket socket(io);
        acceptor.accept(socket);
        char        data[1024] {};
        std::size_t n   = socket.read_some(boost::asio::buffer(data));
        std::string out = "echo(asio-st): " + std::string(data, n);
        boost::asio::write(socket, boost::asio::buffer(out));
    }
}
