#include "code_generator.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

enum reg_state {
    UNINIT = 0, INIT
};

static int check_num(int gen_num, enum reg_state* registers);
static int check_reg(int reg, enum reg_state* registers);
static void init_regs(int gen_num, enum reg_state* registers);

void gen_code(FILE* f, int len) {
    int i = 0, gen_num;
    enum reg_state registers[4] = {UNINIT, UNINIT, UNINIT, UNINIT};
    assert(f != nullptr);
    srand((unsigned int) time(nullptr));

    while(i < len) {
        gen_num = rand() % (0x100);
        if(check_num(gen_num, registers) == 1) {
            init_regs(gen_num, registers);
            fprintf(f, "0x%x ", gen_num);
            i++;
        }
    }
}

static int check_num(int gen_num, enum reg_state* registers) {
    int indicator;
    assert(registers != nullptr);

    if(((gen_num >> 5) == 0x7) || ((gen_num >> 4) == 0xD) || ((gen_num >> 3) == 0x19)) {
        return 0;
    }

    indicator = 1;
    if((gen_num >> 6) == 0x2) {
        indicator &= check_reg(((gen_num >> 2) & 0x3), registers);
        indicator &= check_reg((gen_num & 0x3), registers);
    }
    if((gen_num >> 2) == 0x31) {
        indicator &= check_reg((gen_num & 0x3), registers);
    }
    return indicator;
}

static int check_reg(int reg, enum reg_state* registers) {
    if(registers[reg] == INIT) {
        return 1;
    }
    return 0;
}

static void init_regs(int gen_num, enum reg_state* registers) {
    assert(registers != nullptr);

    if((gen_num >> 2) == 0x30) {
        registers[gen_num & 0x3] = INIT;
    }
    if((gen_num >> 7) == 0x0) {
        registers[3] = INIT;
    }
}
