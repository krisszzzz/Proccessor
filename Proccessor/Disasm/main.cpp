#include "disasm.h"
#include <string.h>

int main(int argc, char* argv[])
{
    if(argc == 1) {
        __PRINT_ALL_INFO__("Inccorect command line argument count - you should give for the the binary file name to dissassemble\n");
        return CMD_LINE_ARG_ERROR;
    }
    if(argc > 2) {
        __PRINT_ALL_INFO__("Error: too many command line arguments - - you should give for the the binary file name to dissassemble\n");
        return CMD_LINE_ARG_ERROR;
    }
    disasm_bin_file to_disasm = {};
    if(DisasmBinFileCtor(&to_disasm, argv[1]) == DISASM_ERROR) {
        return DISASM_ERROR;
    }
    char* file_name        = strdup(argv[1]);
    char* bin_extension    = strstr(file_name, ".bin");
    *bin_extension = '\0';
    FILE* bin_file  = fopen(file_name, "w");    
    StartDisasm(&to_disasm, bin_file);

}
