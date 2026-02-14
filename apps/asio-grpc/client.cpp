#include <agrpc/asioGrpc.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <grpcpp/create_channel.h>

#include <iostream>

#include "echo.grpc.pb.h"

namespace asio = boost::asio;

int main()
{
    agrpc::GrpcContext grpc_context {std::make_unique<grpc::CompletionQueue>()};

    auto channel = grpc::CreateChannel(
        "127.0.0.1:9501",
        grpc::InsecureChannelCredentials());
    auto stub = netdemo::asio_grpc::EchoService::NewStub(channel);

    asio::co_spawn(
        grpc_context,
        [&]() -> asio::awaitable<void>
        {
            grpc::ClientContext             client_context;
            netdemo::asio_grpc::EchoRequest request;
            request.set_text("hello from asio-grpc client");
            netdemo::asio_grpc::EchoReply reply;

            const grpc::Status status = co_await agrpc::request(
                &netdemo::asio_grpc::EchoService::Stub::AsyncUnaryEcho,
                *stub,
                client_context,
                request,
                reply,
                grpc_context,
                asio::use_awaitable);

            if (status.ok())
            {
                std::cout << reply.text() << '\n';
            }
            else
            {
                std::cerr << "rpc failed: " << status.error_message() << '\n';
            }

            grpc_context.stop();
        },
        asio::detached);

    grpc_context.run();
    return 0;
}
