//#include "cpu.h"
#include "cpu.h"
#include <Windows.h>
//#include "..\\Proccessor_Lib\\murmurhash.h"
#include "..\\Proccessor_Lib\\log.h"

// DEBUG OFF (used to disable log)
//#undef ON_DEBUG
//#define ON_DEBUG(code) 

/// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/// Graphics and RAM & Registers
int Registers[4] = {0};
int RAM[993]     = {0};

const int Width  = 200;
const int Height = 75;
const int Video_memory_amount = Height * Width;                       /// For graphic

char VideoMemory[Video_memory_amount] = {0};

void gotoxy(short x, short y) 
{
    COORD coord = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


/// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/// Working with binary file

static char* StartBinReading(cpu_bin_file* buffer)
{
    char* code = (char*)calloc(buffer->info->st_size, sizeof(char));
    if(code == nullptr) {
        __PRINT_ALL_INFO__("Inccorect memory allocation\n");
        return nullptr;
    }
    fread(code, sizeof(char), buffer->info->st_size, buffer->bin_file);
    return code;
}

void CpuBinFileCtor(cpu_bin_file* to_ctor, const char* file_name, void (*stack_printer)(const int*))
{
    if(to_ctor == nullptr) {
        __PRINT_ALL_INFO__("nullptr binary_file object\n");
        return;
    }
    if(file_name == nullptr) {
        __PRINT_ALL_INFO__("Inccorect file name (nullptr char pointer)\n");
        return;
    }
    if((to_ctor->bin_file = fopen(file_name, "rb")) == nullptr) {
        __PRINT_ALL_INFO__("Something went wrong while opening your bin file\n");
        return;
    }
    to_ctor->info = (struct stat*)calloc(1, sizeof(struct stat));
    
    stat(file_name, to_ctor->info); // Rework
    to_ctor->file_name = file_name;
    if((to_ctor->code  = StartBinReading(to_ctor)) == nullptr)
        return;
    
    to_ctor->code_size = to_ctor->info->st_size;
    to_ctor->stack     = {0};
    CtorStack_int(&to_ctor->stack, 8, stack_printer);

    ON_DEBUG(
    to_ctor->hash = MurmurHash((char*)to_ctor, sizeof(cpu_bin_file) - sizeof(__int64));
    )
    
} 

ON_DEBUG(
int IsCpuBinFileHashUnchanged(cpu_bin_file* to_check)
{
    if(to_check->hash != MurmurHash((char*)to_check, sizeof(cpu_bin_file) - sizeof(__int64))) {
        return 0;
    } else {
        return 1;
    }
}
)
/// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/// Commands implementation

#define DEF_CMD(cmd_code, cmd_name, is_cmd_with_arg, to_input_code, ...)                 \
case (cmd_code) | 0x0:                                                                   \
 {                                                                                       \
    ON_DEBUG(                                                                            \
    SetLogFile(stack_log);                                                               \
    )                                                                                    \
    to_input_code                                                                        \
    ON_DEBUG(                                                                            \
    ResetLogFile();                                                                      \
    SetLogFile(cpu_log);                                                                 \
    CpuDump(to_implement, #cmd_name, ip);                                                \
    ResetLogFile();                                                                      \
    )                                                                                    \
    ip += is_cmd_with_arg*sizeof(int) + 1 + additional_ip_offset;                        \
    additional_ip_offset = 0;                                                            \
    break;                                                                               \
}                                                                           
// REWORK TO CORRECT DUMP

void CpuDump(cpu_bin_file* to_dump, const char* cmd_name, size_t ip)
{
    static int cmd_number    = 0;
    size_t iter_count        = 0;
    cmd_number++;
    PrintToLog("%d command: %s, ip: %d\n", cmd_number, cmd_name, ip);
    for(; iter_count < ip; ++iter_count)
    {
        PrintToLog(" %d |", to_dump->code[iter_count]);  // Using binary operation to correct output in hex form (hex form only for unsigned)
    }
    PrintToLog(" %d*|",  to_dump->code[ip]);                                    //return to hex
    for(iter_count = ip + 1; iter_count < to_dump->code_size; ++iter_count)
    {
        PrintToLog(" %d |", to_dump->code[iter_count]);
    }
    PrintToLog("\n\n");
    for(iter_count = 0; iter_count < 4; iter_count++)
    {
        PrintToLog("%d: %d, ", iter_count + 1, Registers[iter_count]);
    }
    PrintToLog("\n\n");

}

int ImplementCmd(cpu_bin_file* to_implement, FILE* cpu_log, FILE* stack_log) // REWORK LOG_SYSTEM
{
    if(to_implement == nullptr)
    {
        __PRINT_ALL_INFO__("Error, cpu_bin_file is nullptr, the implemention stoped\n")
        return -1;
    }
    size_t ip = 0;    
    for(int vmem_adress = 0; vmem_adress < Video_memory_amount; ++vmem_adress)
    {
        VideoMemory[vmem_adress] = ' ';
    }
    // Check hash
    int additional_ip_offset = 0;
    while(ip < to_implement->code_size)
    {
        switch(to_implement->code[ip] | 0x0)
        {
            #include "..\\Proccessor_Lib\\commands.h"
            default: {
                __PRINT_ALL_INFO__("Error: unknown cmd\n");
                PrintToLog("ip value:%d, unknown command code %d\n", ip, to_implement->code[ip] | 0x0);
                return -1;
            }
        }
    }    
    return 0;
    
}

#define DEF_CMD(cmd_code, cmd_name, ...)        \
PrintToLog("%-3x %s\n", cmd_code, #cmd_name);    

void PrintCmdCode()
{
    #include "..\\Proccessor_Lib\\commands.h"
}
