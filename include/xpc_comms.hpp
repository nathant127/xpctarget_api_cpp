#pragma once

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <chrono>

#include "xpc_msg.hpp"

class Connection {
public:
    Connection(const char* addr, int port);
    Connection() = delete;
    ~Connection();

    int con_connect();

    int con_send(const void* buf, size_t n) const;
    int con_recv(void* buf, size_t n) const;

    // Maybe?
    int SendAsync(Message* msg);
    int WaitOnResponse();
    int CheckResponse();

private:
    int socket_;
    sockaddr_in server_addr_;

};
