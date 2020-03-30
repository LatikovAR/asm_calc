#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "asm_calc.h"
#include "executer.h"
#include "bin_coder.h"

static void make_op(struct reg* mem, struct command com);
static void calc_instr(struct reg* mem, struct command com);
static void write_asm_com(FILE* f, struct command com);
static int read_asm_com(FILE* f, struct command* com);
static int read_op_instr2(FILE* f, struct command* com);
static int read_op_instr1(FILE* f, struct command* com);
static int read_op_type(FILE* f, char* a, int max_size);

void execute_prog(FILE* fin, FILE* fout, struct work_mode mode) {
    int bin_com;
    struct command com;
    struct reg* mem;
    assert(fin != nullptr);

    mem = (struct reg*) calloc(RNUM, sizeof (struct reg));
    assert(mem != nullptr);

    while(1) {
        if(mode.inp_type == HEX) {
            if(fscanf(fin, "0x%x ", &bin_com) != 1) {
                break;
            }
            com = decode_bin_com(bin_com);
        }
        else if(mode.inp_type == ASM) {
            if(read_asm_com(fin, &com) != 1) {
                break;
            }
        }
        else {
            assert(0);
        }

        if(mode.execute == YES) {
            calc_instr(mem, com);
        }

        if(mode.recode == YES) {
            assert(fout != nullptr);
            if(mode.out_type == HEX) {
                fprintf(fout, "0x%x ", code_bin_com(com));
            }
            else if(mode.out_type == ASM) {
                write_asm_com(fout, com);
            }
            else {
                assert(0);
            }
        }
    }

    free(mem);
}

static void calc_instr(struct reg* mem, struct command com) {
    switch (com.opcode) {
    case MOV: {
        mem[D].data = com.instr.data;
        mem[D].condition = INIT;
        break;
    }
    case ADD:
    case SUB:
    case MUL:
    case DIV: {
        make_op(mem, com);
        break;
    }
    case IN: {
        scanf("%d", &(mem[com.instr.rop].data));
        mem[com.instr.rop].condition = INIT;
        break;
    }
    case OUT: {
        assert(mem[com.instr.rop].condition == INIT);
        printf("%d\n", mem[com.instr.rop].data);
        break;
    }
    default: {
        assert(0);
    }
    }
}

static void make_op(struct reg* mem, struct command com) {
    int res, a, b;
    assert(mem[com.instr.ops.rd].condition == INIT);
    assert(mem[com.instr.ops.rs].condition == INIT);
    a = mem[com.instr.ops.rd].data;
    b = mem[com.instr.ops.rs].data;

    switch (com.opcode) {
    case ADD: {
        res = a + b;
        break;
    }
    case SUB: {
        res = a - b;
        break;
    }
    case MUL: {
        res = a * b;
        break;
    }
    case DIV: {
        res = a / b;
        break;
    }
    default: {
        assert(0);
    }
    }

    mem[com.instr.ops.rd].data = res;
}

static void write_asm_com(FILE* f, struct command com) {
    assert(f != nullptr);
    switch (com.opcode) {
    case MOV: {
        fprintf(f, "MOVI %d\n", com.instr.data);
        break;
    }
    case ADD: {
        fprintf(f, "ADD %c, %c\n", 'A' + com.instr.ops.rd - A, 'A' + com.instr.ops.rs - A);
        break;
    }
    case SUB: {
        fprintf(f, "SUB %c, %c\n", 'A' + com.instr.ops.rd - A, 'A' + com.instr.ops.rs - A);
        break;
    }
    case MUL: {
        fprintf(f, "MUL %c, %c\n", 'A' + com.instr.ops.rd - A, 'A' + com.instr.ops.rs - A);
        break;
    }
    case DIV: {
        fprintf(f, "DIV %c, %c\n", 'A' + com.instr.ops.rd - A, 'A' + com.instr.ops.rs - A);
        break;
    }
    case IN: {
        fprintf(f, "IN %c\n", 'A' + com.instr.rop - A);
        break;
    }
    case OUT: {
        fprintf(f, "OUT %c\n", 'A' + com.instr.rop - A);
        break;
    }
    default: {
        assert(0);
    }
    }
}

static int read_asm_com(FILE* f, struct command* com) {
    char a[8];
    int wrong_com = 0;
    assert(f != nullptr);
    if(read_op_type(f, a, 8) != 1) {
        return 0;
    }

    if(strcmp(a, "MOVI") == 0) {
        com->opcode = MOV;
        if(fscanf(f, "%d\n", &(com->instr.data)) != 1) {
            wrong_com = 1;
        }
    }
    else if(strcmp(a, "ADD") == 0) {
        com->opcode = ADD;
        wrong_com = read_op_instr2(f, com);
    }
    else if(strcmp(a, "SUB") == 0) {
        com->opcode = SUB;
        wrong_com = read_op_instr2(f, com);
    }
    else if(strcmp(a, "MUL") == 0) {
        com->opcode = MUL;
        wrong_com = read_op_instr2(f, com);
    }
    else if(strcmp(a, "DIV") == 0) {
        com->opcode = DIV;
        wrong_com = read_op_instr2(f, com);
    }
    else if(strcmp(a, "IN") == 0) {
        com->opcode = IN;
        wrong_com = read_op_instr1(f, com);
    }
    else if(strcmp(a, "OUT") == 0) {
        com->opcode = OUT;
        wrong_com = read_op_instr1(f, com);
    }
    else {
        wrong_com = 1;
    }

    if(wrong_com == 1) {
        assert(0 && "wrong input asm command");
    }
    return 1;
}

static int read_op_instr2(FILE* f, struct command* com) {
    char a;
    assert(f != nullptr);
    if((fscanf(f, "%c, ", &a) != 1) || (a < 'A') || (a >= ('A' + RNUM))) {
        return 1;
    }
    com->instr.ops.rd = (enum reg_t) (a - 'A');
    if((fscanf(f, "%c\n", &a) != 1) || (a < 'A') || (a >= ('A' + RNUM))) {
        return 1;
    }
    com->instr.ops.rs = (enum reg_t) (a - 'A');
    return 0;
}

static int read_op_instr1(FILE* f, struct command* com) {
    char a;
    assert(f != nullptr);
    if((fscanf(f, "%c\n", &a) != 1) || (a < 'A') || (a >= ('A' + RNUM))) {
        return 1;
    }
    com->instr.rop = (enum reg_t) (a - 'A');
    return 0;
}

static int read_op_type(FILE* f, char* a, int max_size) {
    int i;
    for(i = 0; i < max_size; i++) {
        if(fscanf(f, "%c", &(a[i])) != 1) {
            if(i == 0) {
                return 0;
            }
            assert(0 && "wrong input asm command");
        }
        if(a[i] == ' ') {
            a[i] = '\0';
            break;
        }
    }
    if(i >= max_size) {
        assert(0 && "wrong input asm command");
    }
    return 1;
}
