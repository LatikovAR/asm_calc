#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "executer.h"

//input parametres for main:
//first of all you should input file with program:
//use -h if input code in hex format, or use -c if it is asm code
//after you should input name of reading file
//result:
//input -o and output filename if you want to get program in other format
//input -e if you want to execute program (stdin and stdout for numbers)
//you can use both parametres in any order

int main(int argc, char** argv) {
    FILE* fin = nullptr;
    FILE* fout = nullptr;
    struct work_mode mode;
    int i;
    assert((argc > 2) && "no input parametres");

    if(strcmp(argv[1], "-c") == 0) {
        mode.inp_type = ASM;
    }
    else if(strcmp(argv[1], "-h") == 0) {
        mode.inp_type = HEX;
    }
    else{
        assert(0 && "wrong input type");
    }

    fin = fopen(argv[2], "r");
    assert((fin != nullptr) && "no input file");

    mode.recode = NO;
    mode.execute = NO;
    i = 3;
    while(i < argc) {
        if(strcmp(argv[i], "-e") == 0) {
            mode.execute = YES;
            i++;
        }
        else if(strcmp(argv[i], "-o") == 0) {
            mode.recode = YES;
            i++;
            if(i >= argc) {
                assert(0 && "no output filename");
            }
            if(fout != nullptr) {
                fclose(fout);
            }
            fout = fopen(argv[i], "w");
            assert(fout != nullptr);
            i++;
        }
        else {
            assert(0 && "wrong command");
        }
    }

    execute_prog(fin, fout, mode);
    fclose(fin);
    fclose(fout);
    return 0;
}
