#include "xpc_comms.hpp"

Connection::Connection() {
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ == -1) {
        throw std::runtime_error("Failed to create unix socket");
    }
}
Connection::~Connection() {
    close(socket_);
}

int Connection::con_connect(const char* addr, int port) {
    server_addr_.sin_family = AF_INET;
    server_addr_.sin_port = htons(port);
    server_addr_.sin_addr.s_addr = inet_addr(addr);

    int ret;
    for (int i = 0; i < 10; i++) {
        ret = connect(socket_, reinterpret_cast<sockaddr*>(&server_addr_), sizeof(server_addr_));
        if (ret != -1) {
            break;

        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (ret == -1) {
        throw std::runtime_error{"Failed to connect"};
    }

    // TODO: Handle connection messages, i think it pings or something?

    return 0;
}

int Connection::con_send(const void* buf, size_t n) const {
    return send(socket_, buf, n, 0);
}

int Connection::con_recv(void* buf, size_t n) const {
    return recv(socket_, buf, n, 0);
}