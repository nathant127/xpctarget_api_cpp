#include "xpc_comms.hpp"

#define DO_BE_CONV 1

constexpr uint8_t MAGIC_NUM1 = 0x4e;
constexpr uint8_t MAGIC_NUM2 = 0xcb;

Message::Message(uint8_t cmd, uint8_t subcmd) {
    payload.resize(8, std::byte{0x00});
    payload[0] = static_cast<std::byte>(MAGIC_NUM1);
    payload[1] = static_cast<std::byte>(MAGIC_NUM2);
    payload[2] = static_cast<std::byte>(cmd);
    payload[3] = static_cast<std::byte>(subcmd);

    cmd_ = cmd;
    subcmd_ = subcmd;
}

Message::~Message() {

}

template <typename DataType>
void Message::push(const std::vector<DataType> buf) {
    push(buf.data(), buf.size());
}

template <typename DataType>
void Message::push(const DataType* buf, size_t buf_size) {
    size_t buf_byte_size = buf_size * sizeof(DataType);
    size_t prefix_length = 0;
    size_t next_size = get_next_size();

    if (buf_size == 0) {
        throw std::logic_error("Tried to push empty buffer");
    }

    // if (next_size == 0) {
    //     throw std::logic_error("tried to push when payload is done");
    // }
    // if (next_size != -1 && 
    //         next_size != buf_byte_size) 
    // {
    //     std::ostringstream msg;
    //     msg << "tried to push buffer with incorrect size: " << buf_byte_size << ", expected " << next_size;
    //     throw std::logic_error("");
    // }

    if (next_size == -1) {
        prefix_length = 4;
    }

    // Would this push exceed the first payloads max size?
    if (!is_payload1_full && (payload.size() + buf_byte_size + prefix_length) > payload1_maxsize) {
        is_payload1_full = true;
        payload1_finalsize = payload.size();
    }

    // Resize and get the write position of the vector
    size_t payload_original_size = payload.size();
    payload.resize(payload.size() + buf_byte_size + prefix_length);
    std::byte* payload_write_ptr = payload.data() + payload_original_size;

    // Add length prefix if needed
    if (next_size == -1) {
        int32_t buf_size_int = buf_size;
        memcpy(payload_write_ptr, &buf_size_int, sizeof(int32_t));
    }
    // Add data
    memcpy(reinterpret_cast<void*>(payload_write_ptr), reinterpret_cast<const void*>(buf), buf_byte_size);
}

int Message::send(const Connection& con) {
    read_idx = 0;

    void* p1;
    void* p2;
    size_t p1_len, p2_len;

    get_tx_payloads(p1, p1_len, p2, p2_len);

    if (con.con_send(p1, p1_len) == -1)
    {
        return 1;
    }
    if (p2_len != 0 && 
        con.con_send(p2, p2_len) == -1) 
    {
        return 1;
    }

    // Handle possibilty of no response expected
    if (cmd_table.at({cmd_, subcmd_}).expect_response == false) {
        res_data.clear();
        return 0;
    }


    if (con.con_recv(&res_header, sizeof(res_header)) != sizeof(res_header)) {
        return 2;
    }

    if (res_header.size == 0) {
        res_data.clear();
        return 0;
    }

    res_data.resize(res_header.size);
    if (con.con_recv(res_data.data(), res_header.size) != res_header.size) {
        return 2;
    }

    
    return 0;
}

template <typename Type>
void Message::read(Type& x) {
    x = *(reinterpret_cast<Type>(&res_data[read_idx]));
    read_idx += sizeof(Type);
}
template<typename Type>
void Message::read(std::vector<Type>& vec) {
    vec.clear();

    int32_t size;
    memcpy(&size, &res_data[read_idx], sizeof(size));
    read_idx += sizeof(size);

    vec.resize(size);
    memcpy(vec.data(), &res_data[read_idx], size);
}

void Message::get_tx_payloads(void*& p1, size_t& p1len, void*& p2, size_t& p2len) {
    if (!is_payload1_full) {
        p1 = payload.data();
        p1len = payload.size();
        p2 = nullptr;
        p2len = 0;
    }
    else {
        p1 = payload.data();
        p1len = payload1_finalsize;
        p2 = payload.data() + payload1_finalsize;
        p2len = payload.size() - payload1_finalsize;
    }

    return;
}


void Message::clear() {
    cmd_ = 0;
    subcmd_ = 0;
    payload.clear();
    is_payload1_full = false;
    payload1_finalsize = 0;
}

size_t get_next_size() {
    // Stub implementation
    return 0;
}