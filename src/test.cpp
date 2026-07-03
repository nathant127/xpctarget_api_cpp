#include "xpc_msg.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
    Connection con("127.0.0.1", 50505);
    con.con_connect();

    Message msg(0xc8, 0x8c);

    std::string example_file_contents = "Hello World\n This is a message, please respond\n";
    msg.push(example_file_contents.data(), example_file_contents.length() + 1);
    msg.send(con);
    
}