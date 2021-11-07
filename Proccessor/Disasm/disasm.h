#if !defined DISASM_INCLUDED
#define DISASM_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "../proccessor_lib/log.h"

#define DISASM_ERROR        -909
#define CMD_LINE_ARG_ERROR  -101

struct disasm_bin_file 
{
    FILE*       bin_file;
    const char* file_name;
    int*        code;
    size_t      code_size;
};

int DisasmBinFileCtor(disasm_bin_file* to_ctor, const char* bin_to_open_name);

int IsDisasmBinFileInvalid(disasm_bin_file* to_disasm);

int StartDisasm(disasm_bin_file* to_disasm, FILE* destination);
#endif