
#include "disasm.h"

#define REG_RESTORE(reg_name, register_code)                      \
case register_code: {                                             \
    return #reg_name;                                             \
}   

static const char* RestoreRegName(const int reg_code)
{
    switch(reg_code) {
        REG_RESTORE(ax, 1);
        REG_RESTORE(bx, 2);
        REG_RESTORE(cx, 3);
        REG_RESTORE(dx, 4);
    }
    return nullptr;
}

int DisasmBinFileCtor(disasm_bin_file* to_ctor, const char* bin_to_open_name)
{
    if(to_ctor == nullptr) {
        __PRINT_ALL_INFO__("Error: nullptr object cannot be constructed\n");
        return DISASM_ERROR;
    }
    if(bin_to_open_name == nullptr) {
        __PRINT_ALL_INFO__("Binary file name is nullptr\n");
        return DISASM_ERROR;
    }

    FILE* bin_to_open = fopen(bin_to_open_name, "rb");

    if(bin_to_open == nullptr) {
        __PRINT_ALL_INFO__("Failed to open file - file name is %s", bin_to_open_name);
	    return DISASM_ERROR;
    }

    fseek(bin_to_open, 0, SEEK_END);
    size_t code_size = ftell(bin_to_open);
    fseek(bin_to_open, 0, SEEK_SET);

    int* code = (int*)calloc(code_size / sizeof(int), sizeof(int));
    fread(code, sizeof(int), code_size / sizeof(int), bin_to_open);
    /// Check
    to_ctor->bin_file = bin_to_open;
    to_ctor->code = code;
    to_ctor->code_size = code_size / sizeof(int);
    to_ctor->file_name = bin_to_open_name;    
    
    return 0;
}

int IsDisasmBinFileInvalid(disasm_bin_file* to_disasm)
{
    if(to_disasm == nullptr) {
        __PRINT_ALL_INFO__("Nullptr object, we cannot validate this object\n");
        return -1;
    }
    int disasm_errno = 0;
    disasm_errno    += (to_disasm->bin_file == nullptr);  /// If the statement true, the disasm_errno increase his value. After returning 
    disasm_errno    += (to_disasm->code == nullptr);      /// the function will detect this. So if all statement is false, the disasm_errno value will be zero.
    disasm_errno    += (to_disasm->file_name == nullptr); /// That means that all test succesfully went through
    disasm_errno    += (to_disasm->code_size == 0 || to_disasm->code_size == -1); // Check this one
    return disasm_errno;
}
///  cmd_code cmd_name with_double_arg with_mem_treatment with_label_treatment with_reg_treatment code
#define DEF_CMD(cmd_code, cmd_name, with_double_arg, with_mem_treatment, with_label_treatment, with_reg_treatment, ...)\
case cmd_code: {                                                                                                       \
    if(with_double_arg) {                                                                                              \
        double to_output = *(double*)(to_disasm->code + ip + 1);                                                       \
        fprintf(destination, "%s %lf\n", #cmd_name, to_output);                                                        \
        ip += 2;                                                                                                       \
        break;                                                                                                         \
    }                                                                                                                  \
    if(with_mem_treatment) {                                                                                           \
        fprintf(destination, "%s [%d]", #cmd_name, to_disasm->code[++ip]);                                             \
        if(to_disasm->code[ip] >= 993) {                                                                               \
            fprintf(destination, " %c\n", to_disasm->code[++ip]);                                                      \
        } else {                                                                                                       \
            fprintf(destination, "\n");                                                                                \
        }                                                                                                              \
        break;                                                                                                         \
    }                                                                                                                  \
    if(with_label_treatment) {                                                                                         \
        fprintf(destination, "%s %d\n", #cmd_name, to_disasm->code[++ip]);                                             \
        break;                                                                                                         \
    }                                                                                                                  \
    if(with_reg_treatment) {                                                                                           \
        fprintf(destination, "%s %s\n", #cmd_name, RestoreRegName(to_disasm->code[++ip]));                             \
        break;                                                                                                         \
    }                                                                                                                  \
    fprintf(destination, "%s\n", #cmd_name);                                                                           \
    break;                                                                                                             \
}


int StartDisasm(disasm_bin_file* to_disasm, FILE* destination) 
{
    if(destination == nullptr) {
        __PRINT_ALL_INFO__("Nullptr object\n");
        return DISASM_ERROR;
    }
    if(!IsDisasmBinFileInvalid(to_disasm)) {
        for(size_t ip = 0; ip < to_disasm->code_size; ++ip)
        {
            switch (to_disasm->code[ip])
            {
                #include "../Proccessor_Lib/commands.h"
                default: {
                    __PRINT_ALL_INFO__("Inccorect command, command code - %d, ip - %llu", to_disasm->code[ip], ip);
                    return DISASM_ERROR;
                }
            }
        }
    } 
    return DISASM_ERROR;
}
