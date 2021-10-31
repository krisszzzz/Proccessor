#ifndef   COMPILATOR_INCLUDED
#define   COMPILATOR_INCLUDED

#define ERROR_CONSTRUCTION -1
#define ALREADY_COMPILATED  1
#define ERROR_CMD          -15
#define ASM_ERROR  -1000 - 7

#define DEBUG

#include <sys\stat.h>
#include "..\\Proccessor_Lib\\textlib.h"
#include "..\\Proccessor_Lib\\defines.h"
///

struct asm_file {
	FILE*             src_file;
	const char*       file_name;
	struct stat 	  info;
	string*           strings;
	size_t            strings_num;
	int               is_initialized;
	void* 			  binary_file;
	ON_DEBUG(
	unsigned __int64  hash;
	)
	
};

struct binary_file {								
	FILE*        	 bin_file;						
	struct stat 	 info;							
	const  char* 	 file_name;						
	ON_DEBUG(										
	unsigned __int64 hash;							
	)												
};

int  AsmFileCtor           (asm_file* to_ctor,     
                                    const char*       file_name_to_open);
                                    
int  IsAsmFileHashUnchanged(asm_file* to_check   );

void AsmFileDtor           (asm_file* to_destruct);

void MakeStringsFromAsmFile(asm_file* source     );

char CompareCommands(const char* cmd, const int cmd_length, int* cmd_arg_num);

int AssembleCmd(asm_file* to_compilate);

#define binary_file

#endif