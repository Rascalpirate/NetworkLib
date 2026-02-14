#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

int main()
{
    int listen_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0)
        return 1;

    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr {};
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(9001);

    if (bind(listen_fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0)
        return 1;
    if (listen(listen_fd, 16) < 0)
        return 1;

    std::cout << "single-thread blocking server on :9001\n";
    while (true)
    {
        int client = accept(listen_fd, nullptr, nullptr);
        if (client < 0)
            continue;
        char    buf[1024] {};
        ssize_t n = read(client, buf, sizeof(buf) - 1);
        if (n > 0)
        {
            std::string msg(buf, static_cast<size_t>(n));
            std::string out = "echo(st): " + msg;
            write(client, out.data(), out.size());
        }
        close(client);
    }
}
