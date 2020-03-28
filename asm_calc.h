#ifndef ASM_CALC_H
#define ASM_CALC_H

enum reg_cond_t {
    UNINIT = 0, INIT
};

struct reg {
    int data;
    enum reg_cond_t condition;
};

enum reg_t {
    A = 0, B, C, D, RNUM
};

enum opcode_t {
    MOV = 0, ADD, SUB, MUL, DIV, IN, OUT, OPNUM
};

union instr_t {
    struct { enum reg_t rd, rs; } ops;
    enum reg_t rop;
    int data;
};

struct command {
    enum opcode_t opcode;
    union instr_t instr;
};

#endif // ASM_CALC_H
