#include "main_functions.h"

int main(int argc, char* argv[])
{
    
    FILE* cpu_log           = fopen("cpu_log.txt",   "w");
    FILE* stack_log         = fopen("stack_log.txt", "w");
    SetLogFile(stack_log);
    SetFontSize();

    char* bin_file_name     = nullptr;
    cpu_bin_file bin_source = {0};

    if(MainArgProccessing(argc, argv, &bin_file_name) != 0) {
        return -1;
    }    
    cpu proccessor = {0};
    CpuCtor(&proccessor);

    CpuBinFileCtor(&bin_source, bin_file_name, printer);
//    PrintCmdCode();
    ImplementCmd(&bin_source, &proccessor, cpu_log, stack_log);
    CpuDtor(&proccessor);
}