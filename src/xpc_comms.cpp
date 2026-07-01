#include "xpc_comms.hpp"

Connection::Connection(const char* addr, int port) {
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ == -1) {
        throw std::runtime_error("Failed to create unix socket");
    }
    server_addr_.sin_family = AF_INET;
    server_addr_.sin_port = htons(port);
    server_addr_.sin_addr.s_addr = inet_addr(addr);
}
Connection::~Connection() {
    close(socket_);
}

int Connection::Connect() {
    for (int i = 0; i < 10; i++) {
        int ret = connect(socket_, reinterpret_cast<sockaddr*>(&server_addr_), sizeof(server_addr_));
        if (ret != -1) {
            break;

        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // TODO: Handle connection messages, i think it pings or something?

    return 0;
}

int Connection::Send(Message& msg) {
    void* p1;
    void* p2;
    size_t p1_len, p2_len;

    msg.get_tx_payloads(p1, p1_len, p2, p2_len);

    if (send(socket_, p1, p1_len, 0) == -1)
    {
        return 1;
    }
    if (p2_len != 0 && 
        send(socket_, p2, p2_len, 0) == -1) 
    {
        return 1;
    }

    // TODO: Handle Response

    return 0;
}

// Stubs for now, maybe forever
int Connection::SendAsync(Message* msg) {
    return 0;
}
int Connection::WaitOnResponse() {
    return 0;
}
int Connection::CheckResponse() {
    return 0;
}
