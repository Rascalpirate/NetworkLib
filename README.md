<!--
 * @Author: error: git config user.name & please set dead value or install git
 * @Date: 2026-02-14 12:33:43
 * @LastEditors: error: git config user.name & please set dead value or install git
 * @LastEditTime: 2026-02-14 13:34:50
 * @FilePath: /MyCoder/CodeX/network.md
 * @Description:
-->

使用cmake 组织项目，开发 demo apps, 其中包含对网络编程使用的 demo. apps 包括：

1. ubuntu 平台上，
    1.1. 使用 raw tcp socket 开发的、基于同步阻塞IO的单线程 server 和 client app;
    1.2. 使用 raw tcp socket 开发的、基于同步阻塞IO的多线程 server 和 client app;
    1.2. 使用 raw tcp socket 开发的、基于同步阻塞IO的线程池 server 和 client app;
    1.3. 使用 epoll 开发的非阻塞的单线程 server 和 client app;

2. windows 平台上，
    2.1 基于 iocp 的 server 和 client app;

3. 跨 windows 和 ubuntu 平台的，基于 boost asio 的：
    3.1. 单线程 server 和 client app;
    3.2. 多线程 server 和 client app;
    3.2. 线程池 server 和 client app;

4. 基于 grpc 四种模式的 server 和 client app;

5. 基于 c++ 20 的 coroutine 的 server 和 client app;
