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
    Fmt get_next_fmt();
    void get_tx_payloads(void*& p1, size_t& p1len, void*& p2, size_t& p2len);
    
    private:
    const size_t payload1_maxsize = 112;
    std::vector<std::byte> payload;
    bool is_payload1_full = false;
    size_t payload1_finalsize;
    
    uint8_t cmd_;
    uint8_t subcmd_;
    Fmt* next_tx_fmt_ptr_;

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

enum class Fmt : uint8_t {
    NONE,
    CHAR,
    BYTE,
    I16,
    U16,
    I32,
    U32,
    I64,
    U64,
    F32,
    F64,
    STR,
    ARR_BYTE,
    ARR_I16,
    ARR_U16,
    ARR_I32,
    ARR_U32,
    ARR_I64,
    ARR_U64,
    ARR_F32,
    ARR_F64,
};

inline bool is_fmt_dyn(Fmt fmt) {
    switch (fmt){
        case Fmt::STR:
        case Fmt::ARR_BYTE:
        case Fmt::ARR_I16:
        case Fmt::ARR_U16:
        case Fmt::ARR_I32:
        case Fmt::ARR_U32:
        case Fmt::ARR_I64:
        case Fmt::ARR_U64:
        case Fmt::ARR_F32:
        case Fmt::ARR_F64:
            return true;
        default:
            return false;
    }
}

inline size_t fmt_size(Fmt fmt) {
    switch (fmt){
        case Fmt::CHAR:
        case Fmt::BYTE:
            return 1;
        case Fmt::I16:
        case Fmt::U16:
            return 2;
        case Fmt::I32:
        case Fmt::U32:
        case Fmt::F32:
            return 4;
        case Fmt::I64:
        case Fmt::U64:
        case Fmt::F64:
            return 8;
        case Fmt::STR:
        case Fmt::ARR_BYTE:
        case Fmt::ARR_I16:
        case Fmt::ARR_U16:
        case Fmt::ARR_I32:
        case Fmt::ARR_U32:
        case Fmt::ARR_I64:
        case Fmt::ARR_U64:
        case Fmt::ARR_F32:
        case Fmt::ARR_F64:
            return -1;
        case Fmt::NONE:
        default:
            return 0;
    }
}


struct CmdInfo {
    bool expect_response;
    Fmt tx_fmt[5];
    Fmt rx_fmt[5];
};


std::unordered_map<Cmd, CmdInfo> cmd_table = {
    {
        {0x00, 0x01}, {false, {Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE}, {Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE}}
    },
};

