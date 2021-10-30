#if !defined CPU_INCLUDED 

#define CPU_INCLUDED

typedef long long ssize_t;
#include "sys\stat.h"
#include "..\\..\\Stack\\Stack.h"
#include "..\\..\\Stack\\DefStack.h"
//#include "..\\..\\Stack\\Stack.cpp"

#define DEBUG
#include "..\\Proccessor_lib\\defines.h"

#ifdef MAIN
Def_Stack_T(int)
#else 
Stack_T(int)
#endif

#include "..\\..\\Stack\\Stack.cpp"

#undef  ON_DEBUG
#define ON_DEBUG(code, ...) code														

struct cpu_bin_file {								
	FILE*        	 bin_file;						
	struct stat* 	 info;							// Rework useless pointers
	stack_int		 stack;
	const  char* 	 file_name;						
	char*			 code;
	size_t			 code_size;
	ON_DEBUG(										
	unsigned __int64 hash;							
	)												
};

void CpuDump(cpu_bin_file* to_dump, const char* cmd_name, size_t ip);

void CpuBinFileCtor(cpu_bin_file* to_ctor, const char* file_name, void (*stack_printer)(const int*) = nullptr);

ON_DEBUG(
int IsCpuBinFileHashUnchanged(cpu_bin_file* to_check);
)
int  ImplementCmd(cpu_bin_file* to_implement, FILE* cpu_log, FILE* stack_log); // REWORK LOG_SYSTEM
void CpuDump(cpu_bin_file* to_dump, const char* cmd_name, size_t ip);
void PrintCmdCode();
#endif