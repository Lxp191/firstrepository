#include <string>
#include <iostream>
#include <grpcpp/grpcpp.h>
#include "test.pb.h"
#include "test.grpc.pb.h"

class TestClient
{
public:
    explicit TestClient(std::shared_ptr<grpc::Channel> channel) : stub_(example::TestService::NewStub(channel)) {}

    void Run()
    {
        example::Test request;
        request.set_name("hello world");
        request.add_age(1);
        request.add_age(2);
        request.add_age(3);
        example::Test response;
        grpc::ClientContext context;
        grpc::Status status = stub_->TestMethod(&context, request, &response);
        std::cout << response.name() << std::endl;
        for (int i = 0; i < response.age_size(); i++)
        {
            std::cout << response.age(i) << std::endl;
        }
    }

private:
    std::unique_ptr<example::TestService::Stub> stub_;
};

int main(int argc, char *argv[])
{
    TestClient testClient(grpc::CreateChannel("127.0.0.1:9091", grpc::InsecureChannelCredentials()));
    testClient.Run();
    return 0;
}