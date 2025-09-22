#include <string>
#include <iostream>
#include <grpcpp/grpcpp.h>
#include "test.pb.h"
#include "test.grpc.pb.h"

class TestServiceImpl final : public example::TestService::Service
{
public:
    grpc::Status TestMethod(grpc::ServerContext *context, const example::Test *request, example::Test *response) override
    {
        std::cout << request->name() << std::endl;
        for (int i = 0; i < request->age_size(); i++)
        {
            std::cout << request->age(i) << std::endl;
        }

        response->set_name("hello,this is server!");
        return grpc::Status::OK;
    }
};

int main(int argc, char *argv[])
{
    TestServiceImpl service;
    grpc::ServerBuilder builder;
    builder.AddListeningPort("127.0.0.1:9091", grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    server->Wait();
    return 0;
}