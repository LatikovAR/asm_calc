#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "executer.h"
#include "code_generator.h"

//input parametres for main:
//use -i if you want to input existing file
//after use -h if input code in hex format, or use -c if it is asm code
//then you should input name of reading file
//also you can use -g, number of commands and "filename" to generate hex file with this number of commands
//result:
//input -r to get recode file
// after use -h for hex or -c for asm and output filename if you want to get program in some format
//input -e if you want to execute program (stdin and stdout for numbers)
//you can use all parametres in any order

static int scan_num(char* a);

int main(int argc, char** argv) {
    FILE* fin = nullptr;
    FILE* fout = nullptr;
    struct work_mode mode;
    int i;
    assert((argc > 1) && "no input parametres");
    mode.recode = NO;
    mode.execute = NO;

    i = 1;
    while(i < argc) {
        if(strcmp(argv[i], "-i") == 0) {
            assert((fin == nullptr) && "repeating input");
            i++;
            assert((i < argc) && "wrong input parametres");
            if(strcmp(argv[i], "-h") == 0) {
                mode.inp_type = HEX;
            }
            else if(strcmp(argv[i], "-c") == 0) {
                mode.inp_type = ASM;
            }
            else {
                assert(0 && "wrong input parametres");
            }
            i++;
            assert((i < argc) && "wrong input parametres");
            fin = fopen(argv[i], "r");
            assert((fin != nullptr) && "no input file");
            i++;
            continue;
        }
        if(strcmp(argv[i], "-g") == 0) {
            assert((fin == nullptr) && "repeating input");
            mode.inp_type = HEX;
            i++;
            assert((i + 1 < argc) && "wrong input parametres");
            fin = fopen(argv[i + 1], "w+");
            gen_code(fin, scan_num(argv[i]));
            i++;
            i++;
            fseek(fin, 0, SEEK_SET);
            continue;
        }
        if(strcmp(argv[i], "-r") == 0) {
            assert((fout == nullptr) && "repeating out");
            mode.recode = YES;
            i++;
            assert((i < argc) && "wrong input parametres");
            if(strcmp(argv[i], "-h") == 0) {
                mode.out_type = HEX;
            }
            else if(strcmp(argv[i], "-c") == 0) {
                mode.out_type = ASM;
            }
            else {
                assert(0 && "wrong input parametres");
            }
            i++;
            assert((i < argc) && "wrong input parametres");
            fout = fopen(argv[i], "w");
            i++;
            continue;
        }
        if(strcmp(argv[i], "-e") == 0) {
            mode.execute = YES;
            i++;
            continue;
        }
        assert(0 && "wrong input parametres");
    }

    execute_prog(fin, fout, mode);
    fclose(fin);
    fclose(fout);
    return 0;
}

static int scan_num(char* a) {
    int ans, i;
    ans = 0;
    i = 0;
    while(a[i] != '\0') {
        if((a[i] <= '9') && (a[i] >= '0')) {
            ans = ans * 10 + (a[i] - '0');
            i++;
        }
        else {
            assert(0 && "wrong input parametres");
        }
    }
    return ans;
}
