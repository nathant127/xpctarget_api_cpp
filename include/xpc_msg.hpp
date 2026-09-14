#pragma once

#include "xpc_comms.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <unordered_map>
#include <sstream>

enum class Fmt : uint8_t;

class Message {
public:
    Message();
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
    template<typename Type>
    void read(Type arr[]);
    void read(std::string& str);

    
    void reset(uint8_t cmd, uint8_t subcmd);
    
    
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





// Cmd Table is at 18001bb60

/*
    'u<X>' -> Unsigned X
    'X*' -> array of X prefixed by length
    Format Code         Size    Assumed Type
    'F' = 0x46 | 70     ?       ?
    'b' = 0x0B | 11     1       uint8_t
    'c' = 0x0A | 10     1       char
    'd' = 0x1E | 30     4       int32_t | uint32_t
    'f' = 0x28 | 40     8       double
    'i' = 0x14 | 20     2       int16_t | uint16_t
    's' = 0x3C | 60     1       cstring?
    '\0' = 0xC8 | 200   0       None?
*/

struct Cmd {
    uint8_t cmd;
    uint8_t subcmd;
    bool operator==(const Cmd& other) const {
        return cmd == other.cmd && subcmd == other.subcmd;
    }
};

namespace std {
    template <>
    struct hash<Cmd> {
        size_t operator()(const Cmd& c) const noexcept {
            // pack both bytes into one value and hash that
            return std::hash<uint16_t>()(
                (static_cast<uint16_t>(c.cmd) << 8) | c.subcmd
            );
        }
    };
}

struct CmdInfo {
    Fmt tx_fmt[5];
    Fmt rx_fmt[5];
};

inline std::unordered_map<Cmd, CmdInfo> cmd_table = {
    {   // {b*} {}
        {0xc8, 0x8c}, {{Fmt::ARR_BYTE,Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE}, {Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE}} // Load app
    },
    {   // {} {f}
        {0x14, 0x28}, {{Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE}, {Fmt::F64,Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE}} // Get sample time
    },
    {   // {} {f}
        {0x14, 0x0A}, {{Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE}, {Fmt::F64,Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE}} // Get Stop time
    },
    {   // {} {ud}
        {0x14, 0xA1}, {{Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE}, {Fmt::U32,Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE}} // Get status? 
    },
    {   // {ud} {}
        {0x14, 0x9B}, {{Fmt::U32,Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE}, {Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE}} // Send Flag to app? 5 = Start, 1 = Stop
    },
    {   // {} {}
        {0x14, 0xAC}, {{Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE}, {Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE}} // Unknown
    },

    {   // {} {s}
        {0xC8, 0x3C}, {{Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE}, {Fmt::STR,Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE}} // Unknown
    },

    {   // {ud*} {f*}
        {0x28, 0x0A}, {{Fmt::ARR_U32,Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE}, {Fmt::ARR_F64,Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE}} // Get signal data
    },
    {   // {s} {d}
        {0x28, 0x46}, {{Fmt::STR,Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE}, {Fmt::I32,Fmt::NONE,Fmt::NONE,Fmt::NONE,Fmt::NONE}} // Get signal id
    },


};

