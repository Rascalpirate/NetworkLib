#include <arpa/inet.h>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>

static void run_one(int id)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        return;
    sockaddr_in addr {};
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(9002);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    if (connect(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0)
        return;

    std::string msg = "hello from thread " + std::to_string(id);
    write(fd, msg.data(), msg.size());

    char    buf[1024] {};
    ssize_t n = read(fd, buf, sizeof(buf) - 1);
    if (n > 0)
        std::cout << std::string(buf, static_cast<size_t>(n)) << "\n";
    close(fd);
}

int main()
{
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i)
        threads.emplace_back(run_one, i);
    for (auto &t : threads)
        t.join();
    return 0;
}
