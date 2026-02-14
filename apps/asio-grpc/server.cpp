#include <agrpc/asioGrpc.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <grpcpp/server_builder.h>

#include <iostream>

#include "echo.grpc.pb.h"

namespace asio = boost::asio;

int main()
{
    netdemo::asio_grpc::EchoService::AsyncService service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort("0.0.0.0:9501", grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    agrpc::GrpcContext grpc_context {builder.AddCompletionQueue()};
    auto               server = builder.BuildAndStart();
    if (!server)
    {
        std::cerr << "failed to start asio-grpc server\n";
        return 1;
    }

    asio::co_spawn(
        grpc_context,
        [&]() -> asio::awaitable<void>
        {
            while (true)
            {
                grpc::ServerContext             server_context;
                netdemo::asio_grpc::EchoRequest request;
                grpc::ServerAsyncResponseWriter<netdemo::asio_grpc::EchoReply>
                    writer {&server_context};

                const bool ok = co_await agrpc::request(
                    &netdemo::asio_grpc::EchoService::AsyncService::
                        RequestUnaryEcho,
                    service,
                    server_context,
                    request,
                    writer,
                    grpc_context,
                    asio::use_awaitable);
                if (!ok)
                {
                    break;
                }

                netdemo::asio_grpc::EchoReply reply;
                reply.set_text("asio-grpc echo: " + request.text());
                co_await agrpc::finish(
                    writer,
                    reply,
                    grpc::Status::OK,
                    asio::use_awaitable);
            }
        },
        asio::detached);

    std::cout << "asio-grpc unary server listening on :9501\n";
    grpc_context.run();

    server->Shutdown();
    return 0;
}
