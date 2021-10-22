#ifndef     COMPILATOR_INCLUDED
#define     COMPILATOR_INCLUDED

#include "D:\ded_projects\Proccessor\TextLib.h"




struct Commands
{
    char        cmd_code;
    const char* cmd_name;
    int         arg_num;
};




enum ERRORS
{
    kCompilation_Err = -15,
    kErrorCmd = -14
};


char CompareCommands(const char* cmd, const int cmd_length, int* get_cmd_index);

int CompileCmd(compilation_file* to_compilate);

#endif