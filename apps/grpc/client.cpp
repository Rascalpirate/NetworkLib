#include "chat.grpc.pb.h"
#include <grpcpp/grpcpp.h>

#include <iostream>

int main()
{
    auto channel = grpc::CreateChannel(
        "127.0.0.1:9401",
        grpc::InsecureChannelCredentials());
    auto stub = netdemo::ChatService::NewStub(channel);

    grpc::ClientContext  c1;
    netdemo::ChatMessage req1, rsp1;
    req1.set_text("hello unary");
    stub->UnaryEcho(&c1, req1, &rsp1);
    std::cout << rsp1.text() << "\n";

    grpc::ClientContext  c2;
    auto                 reader = stub->ServerStream(&c2, req1);
    netdemo::ChatMessage s;
    while (reader->Read(&s))
        std::cout << s.text() << "\n";

    grpc::ClientContext  c3;
    netdemo::ChatMessage rsp3;
    auto                 writer = stub->ClientStream(&c3, &rsp3);
    for (int i = 0; i < 3; ++i)
    {
        netdemo::ChatMessage m;
        m.set_text("client stream msg " + std::to_string(i));
        writer->Write(m);
    }
    writer->WritesDone();
    writer->Finish();
    std::cout << rsp3.text() << "\n";

    grpc::ClientContext c4;
    auto                rw = stub->BidiStream(&c4);
    for (int i = 0; i < 3; ++i)
    {
        netdemo::ChatMessage m;
        m.set_text("bidi msg " + std::to_string(i));
        rw->Write(m);
        netdemo::ChatMessage back;
        if (rw->Read(&back))
            std::cout << back.text() << "\n";
    }
    rw->WritesDone();
    rw->Finish();
    return 0;
}
