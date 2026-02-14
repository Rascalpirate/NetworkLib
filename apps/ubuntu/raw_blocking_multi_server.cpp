#include <arpa/inet.h>
#include <iostream>
#include <thread>
#include <unistd.h>

static void handle_client(int client)
{
    char    buf[1024] {};
    ssize_t n = read(client, buf, sizeof(buf) - 1);
    if (n > 0)
    {
        std::string out
            = "echo(mt): " + std::string(buf, static_cast<size_t>(n));
        write(client, out.data(), out.size());
    }
    close(client);
}

int main()
{
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0)
        return 1;
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr {};
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(9002);
    if (bind(listen_fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0)
        return 1;
    if (listen(listen_fd, 64) < 0)
        return 1;

    std::cout << "multi-thread blocking server on :9002\n";
    while (true)
    {
        int client = accept(listen_fd, nullptr, nullptr);
        if (client < 0)
            continue;
        std::thread(handle_client, client).detach();
    }
}
