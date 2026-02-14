#include <arpa/inet.h>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <unistd.h>
#include <vector>

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
    addr.sin_port        = htons(9003);
    if (bind(listen_fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0)
        return 1;
    if (listen(listen_fd, 64) < 0)
        return 1;

    std::mutex              m;
    std::condition_variable cv;
    std::queue<int>         q;

    auto worker = [&]
    {
        while (true)
        {
            int client;
            {
                std::unique_lock<std::mutex> lock(m);
                cv.wait(
                    lock,
                    [&]
                    {
                        return !q.empty();
                    });
                client = q.front();
                q.pop();
            }
            char    buf[1024] {};
            ssize_t n = read(client, buf, sizeof(buf) - 1);
            if (n > 0)
            {
                std::string out
                    = "echo(pool): " + std::string(buf, static_cast<size_t>(n));
                write(client, out.data(), out.size());
            }
            close(client);
        }
    };

    std::vector<std::thread> pool;
    for (int i = 0; i < 4; ++i)
        pool.emplace_back(worker);

    std::cout << "thread-pool blocking server on :9003\n";
    while (true)
    {
        int client = accept(listen_fd, nullptr, nullptr);
        if (client < 0)
            continue;
        {
            std::lock_guard<std::mutex> lock(m);
            q.push(client);
        }
        cv.notify_one();
    }
}
