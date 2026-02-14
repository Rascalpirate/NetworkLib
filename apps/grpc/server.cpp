#include "chat.grpc.pb.h"
#include <grpcpp/grpcpp.h>

#include <iostream>

class ChatServiceImpl final : public netdemo::ChatService::Service
{
public:
    grpc::Status UnaryEcho(
        grpc::ServerContext *,
        const netdemo::ChatMessage *req,
        netdemo::ChatMessage       *rsp) override
    {
        rsp->set_text("unary: " + req->text());
        return grpc::Status::OK;
    }

    grpc::Status ServerStream(
        grpc::ServerContext *,
        const netdemo::ChatMessage               *req,
        grpc::ServerWriter<netdemo::ChatMessage> *writer) override
    {
        for (int i = 0; i < 3; ++i)
        {
            netdemo::ChatMessage msg;
            msg.set_text(
                "server-stream[" + std::to_string(i) + "]: " + req->text());
            writer->Write(msg);
        }
        return grpc::Status::OK;
    }

    grpc::Status ClientStream(
        grpc::ServerContext *,
        grpc::ServerReader<netdemo::ChatMessage> *reader,
        netdemo::ChatMessage                     *rsp) override
    {
        netdemo::ChatMessage m;
        int                  count = 0;
        while (reader->Read(&m))
            ++count;
        rsp->set_text(
            "client-stream received messages: " + std::to_string(count));
        return grpc::Status::OK;
    }

    grpc::Status BidiStream(
        grpc::ServerContext *,
        grpc::ServerReaderWriter<netdemo::ChatMessage, netdemo::ChatMessage>
            *stream) override
    {
        netdemo::ChatMessage m;
        while (stream->Read(&m))
        {
            netdemo::ChatMessage rsp;
            rsp.set_text("bidi: " + m.text());
            stream->Write(rsp);
        }
        return grpc::Status::OK;
    }
};

int main()
{
    ChatServiceImpl     service;
    grpc::ServerBuilder builder;
    builder.AddListeningPort("0.0.0.0:9401", grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    auto server = builder.BuildAndStart();
    std::cout << "gRPC four-modes server on :9401\n";
    server->Wait();
}
