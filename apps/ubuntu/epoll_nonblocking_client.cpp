#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        return 1;

    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);

    sockaddr_in addr {};
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(9004);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    connect(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));

    int         epfd = epoll_create1(0);
    epoll_event ev {};
    ev.events  = EPOLLOUT | EPOLLIN;
    ev.data.fd = fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);

    epoll_event events[4];
    bool        sent = false;
    while (true)
    {
        int n = epoll_wait(epfd, events, 4, 3000);
        if (n <= 0)
            break;
        for (int i = 0; i < n; ++i)
        {
            if ((events[i].events & EPOLLOUT) && !sent)
            {
                std::string msg = "hello epoll client";
                write(fd, msg.data(), msg.size());
                sent = true;
            }
            if (events[i].events & EPOLLIN)
            {
                char    buf[1024] {};
                ssize_t r = read(fd, buf, sizeof(buf) - 1);
                if (r > 0)
                    std::cout << std::string(buf, static_cast<size_t>(r))
                              << "\n";
                close(fd);
                return 0;
            }
        }
    }
    close(fd);
    return 0;
}
