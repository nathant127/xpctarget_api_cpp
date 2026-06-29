#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <stdexcept>
#include <cstring>

class Message {
public:
    Message(uint8_t cmd, uint8_t subcmd);
    Message() = delete;
    ~Message();



    template <typename DataType>
    void push(const DataType* buf, size_t buf_size);
    template <typename DataType>
    void push(const std::vector<DataType> buf);

    void get_tx_payloads(void*& p1, size_t& p1len, void*& p2, size_t& p2len);


    void clear();

private:
    size_t get_next_size();

private:
    const size_t payload1_maxsize = 112;
    std::vector<std::byte> payload;
    bool is_payload1_full = false;
    size_t payload1_finalsize;

    uint8_t cmd_;
    uint8_t subcmd_;
};