#include <string.h>
#include "Compilator.h"
#include "D:\ded_projects\Proccessor\log.h"

#define DEF_CMD(cmd_code, cmd_name, arg_num)            \
{cmd_code, #cmd_name, arg_num},      

Commands CMD[] = {
   #include "Commands.h"
};

#define DEF_CMD(cmd_code, ...)                        \
case cmd_code: ;

static void CollisionChecker()
{
    switch(0) {
    #include "Commands.h"
    }
}


int CompileCmd(compilation_file* to_compile)
{
    if(to_compile == nullptr) {
        __PRINT_ALL_INFO__("Inccorect argument - is nullptr\n")
        return kCompilation_Err;
    }
    if(to_compile->is_initialized == 0) {
        __PRINT_ALL_INFO__("Unitialized compilition_file object\n")
        return kCompilation_Err;
    }

    ON_DEBUG(
        if(!IsCompilationFileHashUnchanged(to_compile))
            return kCompilation_Err;
    )

    for(size_t iter_count = 0; iter_count < to_compile->strings_num; ++iter_count)
    {
        char* cmd            = nullptr;
        char  cmd_code       = 0;
        int   command_length = 0;
        long  par            = 0;
        int   cmd_index      = 0;
        sscanf(to_compile->strings[iter_count].line, "%ms %n", &cmd, &command_length);
        cmd_code = CompareCommands(cmd,command_length, &cmd_index);

        if(cmd_code != kErrorCmd) {
            fwrite(&cmd_code, sizeof(char), 1, to_compile->binary_file->file_dst_src);

            if(CMD[cmd_index].arg_num) {
                if(sscanf(to_compile->strings[iter_count].line + command_length, "%ld", &par) <= 0) {
                    __PRINT_ALL_INFO__("Inccorect argument, your put incorrect argument\n")
                    PrintToLog("Your input: %ld", par);
                    return kCompilation_Err;
                } else {
                    fwrite(&par, sizeof(long), 1, to_compile->binary_file->file_dst_src);
                }
            }

        } else {
            __PRINT_ALL_INFO__("Inccorect command, please see the instruction, where all commands showed\n")
            PrintToLog("Your input: %s", cmd);

            return kCompilation_Err;
        }
        free(cmd);
    }
    return 0;
}

char CompareCommands(const char* cmd, const int cmd_length, int* get_cmd_index)
{
    size_t commands_amount = sizeof(CMD) / sizeof(Commands);
    for(size_t cmd_index = 0; cmd_index < commands_amount; ++cmd_index)
    {
        if(strcmp(cmd, CMD[cmd_index].cmd_name) == 0) {
            *get_cmd_index = cmd_index;
            return CMD[cmd_index].cmd_code;
        }
    }

    return kErrorCmd;
}



