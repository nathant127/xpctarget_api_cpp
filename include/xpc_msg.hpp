#pragma once

#include "xpc_comms.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <unordered_map>

class Message {
public:
    Message(uint8_t cmd, uint8_t subcmd);
    Message() = delete;
    ~Message();



    template <typename DataType>
    void push(const DataType* buf, size_t buf_size);
    template <typename DataType>
    void push(const std::vector<DataType> buf);

    int send(const Connection& con);

    template <typename Type>
    void read(Type& x);
    template<typename Type>
    void read(std::vector<Type>& vec);
    void read(std::string& str);

    
    void clear();
    
    
    private:
    size_t get_next_size();
    void get_tx_payloads(void*& p1, size_t& p1len, void*& p2, size_t& p2len);
    
    private:
    const size_t payload1_maxsize = 112;
    std::vector<std::byte> payload;
    bool is_payload1_full = false;
    size_t payload1_finalsize;
    
    uint8_t cmd_;
    uint8_t subcmd_;

    struct {
        uint8_t magic_num1;
        uint8_t magic_num2;
        uint8_t cmd;
        uint8_t subcmd;
        int size;

    } res_header;
    std::vector<std::byte> res_data;
    size_t read_idx;
    
};

struct Cmd {
    uint8_t cmd;
    uint8_t subcmd;
};
struct CmdInfo {
    bool expect_response;
};

std::unordered_map<Cmd, CmdInfo> cmd_table = {
    {
        {0x00, 0x01}, {false}
    },
};