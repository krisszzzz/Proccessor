//#include "cpu.h"
#include "cpu.h"
#include <Windows.h>
#include "../Proccessor_Lib/log.h"  
#include <math.h>

/// DEBUG OFF (used to disable log) (remove commentary code)
//#undef ON_DEBUG
//#define ON_DEBUG(code) 

#define CPU_CONSTRUCTION_FAILED -1
#define CPU_DESTRUCTION_FAILED  -1 

#define MAX_PRECISION 0.001

static int IsEqual(double first_val, double second_val)
{
    return (fabs(first_val - second_val) < MAX_PRECISION) ? 1 : 0;
}
/// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/// Cpu structure with all fuctions



int CpuCtor(cpu* to_ctor)
{
    if(to_ctor == nullptr) {
        __PRINT_ALL_INFO__("Error: nullptr object cannot be constructed\n");
        return CPU_CONSTRUCTION_FAILED;
    }

    cpu null_object = {0};
    *to_ctor = null_object;
    to_ctor->VideoMemory = (char*)calloc(SCREEN_HEIGHT_IN_ASCII_SYMBOLS * SCREEN_WIDTH_IN_ASCII_SYMBOLS, sizeof(char));

    return 0;
}

int CpuDtor(cpu* to_dtor)
{
    if(to_dtor == nullptr) {
        __PRINT_ALL_INFO__("Error: nullptr object cannot be destructed\n");
        return CPU_DESTRUCTION_FAILED;
    }
    free(to_dtor->VideoMemory);
    return 0;
}
/// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/// For correct graphic visualisation

static void gotoxy(short x, short y) 
{
    COORD coord = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


/// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/// Working with binary file

static int* StartBinReading(cpu_bin_file* buffer)
{
    int* code = (int*)calloc(buffer->info->st_size / sizeof(int), sizeof(int)); // divided by sizeof(int), because one symbol = one byte, but int = four byte
    if(code == nullptr) {
        __PRINT_ALL_INFO__("Inccorect memory allocation\n");
        return nullptr;
    }
    fread(code, sizeof(int), buffer->info->st_size / sizeof(int), buffer->bin_file);
    return code;
}

void CpuBinFileCtor(cpu_bin_file* to_ctor, const char* file_name, void (*stack_printer)(const double*))
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
    
    to_ctor->code_size = to_ctor->info->st_size / 4;
    to_ctor->stack     = {0};
    ///Create stack with 16 elements
    CtorStack_double(&to_ctor->stack, 16, stack_printer);

    ON_DEBUG(
    to_ctor->hash = MurmurHash((char*)to_ctor, sizeof(cpu_bin_file) - sizeof(__int64));
    )
    
} 

ON_DEBUG(
int IsCpuBinFileHashUnchanged(cpu_bin_file* to_check)
{
    return (to_check->hash != MurmurHash((char*)to_check, sizeof(cpu_bin_file) - sizeof(__int64))) ? 0 : 1;
}
)
/// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/// Commands implementation
///  cmd_code cmd_name with_double_arg with_mem_treatment with_label_treatment with_reg_treatment code_to_input
#define DEF_CMD(cmd_code, cmd_name, with_double_arg, with_mem_treatment, with_label_treatment,  \
                with_reg_treatment, to_input_code, ...)                                         \
case (cmd_code):                                                                                \
 {                                                                                              \
    ON_DEBUG(                                                                                   \
    SetLogFile(stack_log);                                                                      \
    )                                                                                           \
    to_input_code                                                                               \
    ON_DEBUG(                                                                                   \
    ResetLogFile();                                                                             \
    SetLogFile(cpu_log);                                                                        \
    CpuDump(to_implement, proccessor, #cmd_name, ip);                                           \
    ResetLogFile();                                                                             \
    )                                                                                           \
    ip += with_double_arg      * (sizeof(double) / sizeof(int)) + with_mem_treatment +          \
          with_label_treatment  + with_reg_treatment  + 1    + additional_ip_offset;            \
    additional_ip_offset = 0;                                                                   \
    break;                                                                                      \
}                                                                           


void CpuDump(cpu_bin_file* to_dump, cpu* proccessor, const char* cmd_name, size_t ip)
{
    static int cmd_number    = 0;
    size_t iter_count        = 0;
    cmd_number++;
    PrintToLog("%d command: %s, ip: %d\n", cmd_number, cmd_name, ip);
    for(; iter_count < ip; ++iter_count)
    {
        PrintToLog(" %0x |", 255 &  to_dump->code[iter_count]);  // Using binary operation to correct output in hex form (hex form only for unsigned)
        PrintToLog(" %0x |", 255 & (to_dump->code[iter_count] << 8));
        PrintToLog(" %0x |", 255 & (to_dump->code[iter_count] << 16));
        PrintToLog(" %0x |", 255 & (to_dump->code[iter_count] << 24));
    }
    PrintToLog(" %0x*|", 255 & to_dump->code[ip]);                                    //return to hex
    PrintToLog(" %0x |", 255 & (to_dump->code[ip] << 8));
    PrintToLog(" %0x |", 255 & (to_dump->code[ip] << 16));
    PrintToLog(" %0x |", 255 & (to_dump->code[ip] << 24));

    for(iter_count = ip + 1; iter_count < to_dump->code_size; ++iter_count)
    {
        PrintToLog(" %0x |", 255 & to_dump->code[iter_count]);  
        PrintToLog(" %0x |", 255 & (to_dump->code[iter_count] << 8));
        PrintToLog(" %0x |", 255 & (to_dump->code[iter_count] << 16));
        PrintToLog(" %0x |", 255 & (to_dump->code[iter_count] << 24));
    }
    PrintToLog("\n\n");
    for(iter_count = 0; iter_count < REG_AMOUNT; iter_count++)
    {
        PrintToLog("%d: %lf  ", iter_count + 1, proccessor->Registers[iter_count]);
    }
    PrintToLog("\n\n");

}

int ImplementCmd(cpu_bin_file* to_implement, cpu* proccessor, FILE* cpu_log, FILE* stack_log) // REWORK LOG_SYSTEM
{
    if(to_implement == nullptr) {
        __PRINT_ALL_INFO__("Error, cpu_bin_file is nullptr, the implemention stoped\n")
        return -1;
    }
    for(int vmem_adress = 0; vmem_adress < SCREEN_WIDTH_IN_ASCII_SYMBOLS * SCREEN_HEIGHT_IN_ASCII_SYMBOLS; ++vmem_adress)
    {
        proccessor->VideoMemory[vmem_adress] = ' ';
    }
    // Check hash
    size_t ip                   = 0;    
    int    additional_ip_offset = 0;
    while(ip < to_implement->code_size)
    {                         /// Divide by 4, because of int* arithmetics
        switch(to_implement->code[ip])
        {
            #include "../Proccessor_Lib/commands.h"
            default: {
                __PRINT_ALL_INFO__("Error: unknown cmd\nip value:%llu, unknown command code %d\n", ip, to_implement->code[ip]);
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
    #include "../Proccessor_Lib/commands.h"
}
