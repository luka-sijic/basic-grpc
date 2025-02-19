package main

import (
    "context"
    "log"
    "time"

    pb "grpc-demo/proto"  
    "google.golang.org/grpc"
    "google.golang.org/grpc/credentials/insecure"
)

func main() {
    conn, err := grpc.Dial("localhost:50051", grpc.WithTransportCredentials(insecure.NewCredentials()))
    if err != nil {
        log.Fatalf("did not connect: %v", err)
    }
    defer conn.Close()
    c := pb.NewGreeterClient(conn)

    // Create context with timeout
    ctx, cancel := context.WithTimeout(context.Background(), time.Second)
    defer cancel()
    conn.Connect()
    // Call SayHello with HelloRequest, expect HelloResponse (not HelloReply)
    r, err := c.SayHello(ctx, &pb.HelloRequest{Name: "Test"})
    if err != nil {
        log.Printf("Error details: %+v\n", err) 
        log.Fatalf("could not greet: %v", err)
    }
    log.Printf("Message: %s", r.GetMessage())
    log.Printf("Value: %v", r.GetValue())
}
