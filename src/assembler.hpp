#pragma once

// Standard includes
#include <cstdint>
#include <iostream>
#include <vector>
#include <fstream>

using byte_t = uint8_t;

const bool inst = false;
const bool data = true;

// opcodes
const byte_t op_mini_isa_2 = 0x00; // 3 bytes : write data to address at data_2
const byte_t op_mini_isa_4 = 0x10; // 4 bytes : write data to address at data_3
const byte_t op_input = 0x20;      // 1 byte : write one input byte to address 1
const byte_t op_random = 0x30;     // 1 byte : write one input byte to address 0
const byte_t op_line_collision = 0x40;   // 3 bytes : write result to data_2 ???
const byte_t op_point_collision = 0x50;  // 3 bytes : write result to data_2 ???
const byte_t op_border_collision = 0x60; // 2 bytes : write result to data_1 ???
const byte_t op_write_to_screen = 0x70;  // 3 bytes : color = d1; pos = *d2;
const byte_t op_equal_to = 0x80;     // 4 bytes : if (*d1 == *d2) { goto d3; }
const byte_t op_greater_than = 0x90; // 4 bytes : if (*d1 > *d2) { goto d3; }
const byte_t op_big = 0x90;          // Alias of greater than

// opcodes for mini 2-ISA
const byte_t mop2_nop = 0b00;  // nothing
const byte_t mop2_add = 0b01;  // *d2 = *d1 + *d2;
const byte_t mop2_sub = 0b10;  // *d2 = *d1 - *d2;
const byte_t mop2_jump = 0b11; // goto d2;

// opcodes for mini 4-ISA
const byte_t mop4_nop = 0b000;         // nothing
const byte_t mop4_add = 0b001;         // *d2 = *d1 + *d2;
const byte_t mop4_sub = 0b010;         // *d2 = *d1 - *d2;
const byte_t mop4_and = 0b011;         // *d2 = *d1 & *d2;
const byte_t mop4_or = 0b100;          // *d2 = *d1 | *d2;
const byte_t mop4_not = 0b101;         // *d2 = ~(*d2);
const byte_t mop4_right_shift = 0b110; // *d2 = *d1 >> *d2;
const byte_t mop4_left_shift = 0b111;  // *d2 = *d1 << *d2;

struct instruction_t {
    bool is_data = false;
    byte_t opcode_1 = op_mini_isa_2;
    byte_t opcode_2 = (mop2_nop << 2) | mop2_nop;
    byte_t data_1 = 0;
    byte_t data_2 = 0;
    byte_t data_3 = 0;
};

int assemble(const std::vector<instruction_t>& program) {
    std::vector<byte_t> binary;

    for (size_t i = 0; i < program.size(); ++i) {
        auto ins = program.at(i);

        if (ins.is_data) {
            binary.push_back(ins.opcode_1);
            continue;
        }

        if (ins.opcode_1 % 16 != 0) {
            std::cerr << "[" << i
                      << "] opcode_1 must be cleanly divisible by 16, but it's "
                         "actually "
                      << ins.opcode_1 << '\n';
            return 1;
        }
        if (ins.opcode_2 >= 16) {
            std::cerr << "[" << i
                      << "] opcode_2 must be <= 15, but it's actually "
                      << ins.opcode_2 << '\n';
            return 1;
        }

        byte_t opcode = ins.opcode_1 & ins.opcode_2;

        switch (opcode) {
            // Only need one byte.
            case op_input:
            case op_random:
                binary.push_back(opcode);
                break;

            // Only need two bytes.
            case op_border_collision:
                binary.push_back(opcode);
                binary.push_back(ins.data_1);
                break;

            // Only need three bytes.
            case op_mini_isa_2:
            case op_line_collision:
            case op_point_collision:
            case op_write_to_screen:
                binary.push_back(opcode);
                binary.push_back(ins.data_1);
                binary.push_back(ins.data_2);
                break;

            // Only need four bytes.
            case op_mini_isa_4:
            case op_equal_to:
            case op_greater_than:
                binary.push_back(opcode);
                binary.push_back(ins.data_1);
                binary.push_back(ins.data_2);
                binary.push_back(ins.data_3);
                break;

            default:
                std::cerr << "Invalid opcode: " << opcode << '\n';
                return 1;
        }
    }

    std::ofstream file("out.bin", std::ios_base::binary);

    for (byte_t byte : binary) {
        file.put(static_cast<std::ofstream::char_type>(byte));
    }

    return 0;
}
