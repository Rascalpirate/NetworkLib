# NetworkLib Demo Apps

使用 CMake 组织的网络编程 demo 集合，覆盖 Linux、Windows、跨平台 Boost.Asio、gRPC 四种模式，以及 C++20 coroutine。

## 构建

```bash
cmake -S . -B build
cmake --build build -j
```

可选开关：

- `-DNETLIB_BUILD_UBUNTU_DEMOS=ON/OFF`
- `-DNETLIB_BUILD_WINDOWS_IOCP_DEMOS=ON/OFF`
- `-DNETLIB_BUILD_BOOST_ASIO_DEMOS=ON/OFF`
- `-DNETLIB_BUILD_GRPC_DEMOS=ON/OFF`（默认 OFF，需要本机安装 gRPC+Protobuf）
- `-DNETLIB_BUILD_COROUTINE_DEMOS=ON/OFF`
- `-DNETLIB_BUILD_ASIO_GRPC_DEMOS=ON/OFF`（默认 OFF，需要 Boost + gRPC + Protobuf，且会拉取 `Tradias/asio-grpc`）

## Demo 列表

### 1. Ubuntu 平台

- Raw TCP + 同步阻塞 IO + 单线程
  - `ubuntu_raw_blocking_st_server`
  - `ubuntu_raw_blocking_st_client`
- Raw TCP + 同步阻塞 IO + 多线程
  - `ubuntu_raw_blocking_mt_server`
  - `ubuntu_raw_blocking_mt_client`
- Raw TCP + 同步阻塞 IO + 线程池
  - `ubuntu_raw_blocking_pool_server`
  - `ubuntu_raw_blocking_pool_client`
- Epoll + 非阻塞 + 单线程
  - `ubuntu_epoll_nonblocking_server`
  - `ubuntu_epoll_nonblocking_client`

### 2. Windows 平台

- IOCP（示例骨架）
  - `windows_iocp_server`
  - `windows_iocp_client`

### 3. 跨平台 Boost.Asio

- 单线程
  - `asio_st_server`
  - `asio_st_client`
- 多线程
  - `asio_mt_server`
  - `asio_mt_client`
- 线程池
  - `asio_pool_server`
  - `asio_pool_client`

### 4. gRPC 四种模式

- 统一在一对 server/client 中演示：
  - Unary
  - Server Streaming
  - Client Streaming
  - Bidirectional Streaming
- 可执行文件：
  - `grpc_modes_server`
  - `grpc_modes_client`

### 5. C++20 Coroutine

- `coro_server`
- `coro_client`

## 运行示例

先启动 server，再运行对应 client。例如：

```bash
./build/apps/ubuntu/ubuntu_raw_blocking_st_server
./build/apps/ubuntu/ubuntu_raw_blocking_st_client
```


### 6. asio-grpc（Boost.Asio + gRPC）

- `asio_grpc_server`
- `asio_grpc_client`

> 说明：该 demo 使用 `https://github.com/Tradias/asio-grpc`，用于演示基于 `agrpc::GrpcContext` 的 unary RPC 异步写法。


启用 asio-grpc demo 构建示例：

```bash
cmake -S . -B build -DNETLIB_BUILD_ASIO_GRPC_DEMOS=ON
cmake --build build -j
```
