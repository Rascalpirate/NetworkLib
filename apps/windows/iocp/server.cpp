#ifdef _WIN32
    #include <MSWSock.h>
    #include <WinSock2.h>
    #include <Windows.h>
    #include <iostream>
    #pragma comment(lib, "Ws2_32.lib")

int main()
{
    WSADATA wsa {};
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET listen_sock
        = WSASocket(AF_INET, SOCK_STREAM, 0, nullptr, 0, WSA_FLAG_OVERLAPPED);
    sockaddr_in addr {};
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(9010);

    bind(listen_sock, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
    listen(listen_sock, SOMAXCONN);

    HANDLE iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
    CreateIoCompletionPort(reinterpret_cast<HANDLE>(listen_sock), iocp, 0, 0);

    std::cout << "IOCP demo server listening on :9010\n";
    std::cout << "This is a minimal IOCP skeleton; extend with "
                 "AcceptEx/WSARecv for production."
              << std::endl;

    CloseHandle(iocp);
    closesocket(listen_sock);
    WSACleanup();
    return 0;
}
#else
int main()
{
    return 0;
}
#endif
