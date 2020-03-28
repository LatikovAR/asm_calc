#ifndef BIN_CODER_H
#define BIN_CODER_H

#include "asm_calc.h"

struct command decode_bin_com(int bin_com);
int code_bin_com(struct command com);

#endif // BIN_CODER_H
