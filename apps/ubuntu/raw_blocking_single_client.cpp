#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        return 1;

    sockaddr_in addr {};
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(9001);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    if (connect(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0)
        return 1;

    std::string msg = "hello raw blocking single";
    write(fd, msg.data(), msg.size());

    char    buf[1024] {};
    ssize_t n = read(fd, buf, sizeof(buf) - 1);
    if (n > 0)
        std::cout << std::string(buf, static_cast<size_t>(n)) << "\n";
    close(fd);
    return 0;
}
