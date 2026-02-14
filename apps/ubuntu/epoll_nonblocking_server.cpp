#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <unordered_map>

static int set_nonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main()
{
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0)
        return 1;
    set_nonblocking(listen_fd);

    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr {};
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(9004);
    if (bind(listen_fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0)
        return 1;
    if (listen(listen_fd, 128) < 0)
        return 1;

    int         epfd = epoll_create1(0);
    epoll_event ev {};
    ev.events  = EPOLLIN;
    ev.data.fd = listen_fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev);

    epoll_event events[64];
    std::cout << "epoll nonblocking server on :9004\n";
    while (true)
    {
        int n = epoll_wait(epfd, events, 64, -1);
        for (int i = 0; i < n; ++i)
        {
            int fd = events[i].data.fd;
            if (fd == listen_fd)
            {
                while (true)
                {
                    int client = accept(listen_fd, nullptr, nullptr);
                    if (client < 0)
                        break;
                    set_nonblocking(client);
                    epoll_event cev {};
                    cev.events  = EPOLLIN | EPOLLET;
                    cev.data.fd = client;
                    epoll_ctl(epfd, EPOLL_CTL_ADD, client, &cev);
                }
            }
            else
            {
                char    buf[1024] {};
                ssize_t r = read(fd, buf, sizeof(buf) - 1);
                if (r <= 0)
                {
                    close(fd);
                    continue;
                }
                std::string out = "echo(epoll): "
                                + std::string(buf, static_cast<size_t>(r));
                write(fd, out.data(), out.size());
            }
        }
    }
}
