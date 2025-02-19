#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "greet.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using greet::HelloRequest;
using greet::HelloResponse;
using greet::Greeter;

bool luhnChecksum(const std::string& cardNumber) {
    int sum = 0;
    bool alternate = false;

    for (int i = cardNumber.length() - 1;i >= 0;i--) {
        int digit = cardNumber[i] - '0';

        if (alternate) {
            digit *= 2;
            if (digit > 9) {
                digit = (digit % 10) + 1;
            }
        }

        sum += digit;
        alternate = !alternate;
    }
    return (sum % 10 == 0);
}

class GreeterServiceImpl final : public Greeter::Service {
    Status SayHello(ServerContext* context, const HelloRequest* request, HelloResponse* reply) override {
        std::string prefix("Validating card: ");
        std::cout << request->name() << std::endl;
        reply->set_message(prefix + request->name());
        bool val = luhnChecksum(request->name());

        reply->set_value(val);
        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    GreeterServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

int main() {
    RunServer();
    return 0;
}
