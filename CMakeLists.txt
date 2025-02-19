cmake_minimum_required(VERSION 3.10)
project(grpc_example)

find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

add_executable(server
    server.cpp
    greet.pb.cc
    greet.grpc.pb.cc
)

target_link_libraries(server
    gRPC::grpc++
    gRPC::grpc++_reflection
    protobuf::libprotobuf
)
