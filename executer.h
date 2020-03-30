#ifndef EXECUTER_H
#define EXECUTER_H

#include <stdio.h>

enum file_t {
    HEX = 0, ASM
};

enum y_n {
    NO = 0, YES
};

struct work_mode {
    enum file_t inp_type;
    enum file_t out_type;
    enum y_n recode;
    enum y_n execute;
};

void execute_prog(FILE* fin, FILE* fout, struct work_mode mode);

#endif // EXECUTER_H
