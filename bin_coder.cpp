#include "bin_coder.h"
#include <assert.h>

struct command decode_bin_com(int bin_com) {
    struct command com;

    if((bin_com >> 7) == 0x0) {
        com.opcode = MOV;
        com.instr.data = bin_com & 0x7F;
    }
    else if((bin_com >> 4) == 0x8) {
        com.opcode = ADD;
        com.instr.ops.rd = (enum reg_t) ((bin_com >> 2) & 0x3);
        com.instr.ops.rs = (enum reg_t) (bin_com & 0x3);
    }
    else if((bin_com >> 4) == 0x9) {
        com.opcode = SUB;
        com.instr.ops.rd = (enum reg_t) ((bin_com >> 2) & 0x3);
        com.instr.ops.rs = (enum reg_t) (bin_com & 0x3);
    }
    else if((bin_com >> 4) == 0xA) {
        com.opcode = MUL;
        com.instr.ops.rd = (enum reg_t) ((bin_com >> 2) & 0x3);
        com.instr.ops.rs = (enum reg_t) (bin_com & 0x3);
    }
    else if((bin_com >> 4) == 0xB) {
        com.opcode = DIV;
        com.instr.ops.rd = (enum reg_t) ((bin_com >> 2) & 0x3);
        com.instr.ops.rs = (enum reg_t) (bin_com & 0x3);
    }
    else if((bin_com >> 2) == 0x30) {
        com.opcode = IN;
        com.instr.rop = (enum reg_t) (bin_com & 0x3);
    }
    else if((bin_com >> 2) == 0x31) {
        com.opcode = OUT;
        com.instr.rop = (enum reg_t) (bin_com & 0x3);
    }
    else {
        assert(0 && "Wrong command code");
    }
    return com;
}

int code_bin_com(struct command com) {
    switch (com.opcode) {
    case MOV: {
        assert((com.instr.data <= 0x7F) && (com.instr.data >= 0) && "wrong num");
        return ((0x0 << 7) | (com.instr.data));
    }
    case ADD: {
        return ((0x8 << 4) | (com.instr.ops.rd << 2) | (com.instr.ops.rs));
    }
    case SUB: {
        return ((0x9 << 4) | (com.instr.ops.rd << 2) | (com.instr.ops.rs));
    }
    case MUL: {
        return ((0xA << 4) | (com.instr.ops.rd << 2) | (com.instr.ops.rs));
    }
    case DIV: {
        return ((0xB << 4) | (com.instr.ops.rd << 2) | (com.instr.ops.rs));
    }
    case IN: {
        return ((0x30 << 2) | (com.instr.rop));
    }
    case OUT: {
        return ((0x31 << 2) | (com.instr.rop));
    }
    default: {
        assert(0);
    }
    }
}
