#if !defined CPU_INCLUDED 

#define CPU_INCLUDED

#include "sys/stat.h"
#include "../../Stack/Stack.h"
#include "../../Stack/DefStack.h"
#include <math.h>

#define DEBUG
#include "../Proccessor_lib/defines.h"

#ifdef MAIN
Def_Stack_T(double)
#else 
Stack_T(double)
#endif

#undef  ON_DEBUG
#define ON_DEBUG(code, ...) code														

#define REG_AMOUNT                     4
#define RAM_CELL_AMOUNT                993 /// 1 ram cell equal sizeof(int)
#define SCREEN_WIDTH_IN_ASCII_SYMBOLS  200
#define SCREEN_HEIGHT_IN_ASCII_SYMBOLS 75

struct cpu_bin_file {								
	FILE*        	 bin_file;						
	struct stat* 	 info;							
	stack_double	 stack;
	const  char* 	 file_name;						
	int*		 code;
	size_t		 code_size;
	ON_DEBUG(										
	unsigned __int64 hash;							
	)												
};

struct cpu
{
    double Registers[REG_AMOUNT];
    double RAM[RAM_CELL_AMOUNT];
    char*  VideoMemory;

};

int ImplementCmd(cpu_bin_file* to_implement, cpu* proccessor, FILE* cpu_log, FILE* stack_log);

void CpuBinFileCtor(cpu_bin_file* to_ctor, const char* file_name, void (*stack_printer)(const double*));

ON_DEBUG(
int IsCpuBinFileHashUnchanged(cpu_bin_file* to_check);
)

int CpuCtor(cpu* to_ctor);

int CpuDtor(cpu* to_dtor);

void CpuDump(cpu_bin_file* to_dump, cpu* proccessor, const char* cmd_name, size_t ip);

void PrintCmdCode();
#endif
