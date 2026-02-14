#ifdef _WIN32
    #include <WS2tcpip.h>
    #include <WinSock2.h>
    #include <iostream>
    #pragma comment(lib, "Ws2_32.lib")

int main()
{
    WSADATA wsa {};
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET      sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in addr {};
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(9010);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    if (connect(sock, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == 0)
    {
        std::cout << "IOCP demo client connected to :9010\n";
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
#else
int main()
{
    return 0;
}
#endif
