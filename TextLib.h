#ifndef TEXTLIB_INCLUDED
#define TEXTLIB_INCLUDED

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>


#define WINDOWS
#if defined WINDOWS
#define ONLY_ON_WINDOWS(code) code
#else
#define ONLY_ON_WINDOWS(code)  
#endif

#define      CMP_TEXTLIB_MODE
#if defined  CMP_TEXTLIB_MODE
#define      ON_CMP_TEXTLIB_MODE(code) code
#else
#define		ON_CMP_TEXTLIB_MODE(code)  
#endif

#define DEBUG
#if defined DEBUG
#define ON_DEBUG(code) code
#else
#define ON_DEBUG(code) 
#endif

struct string
{
	char* line;
};

ON_CMP_TEXTLIB_MODE(

struct compilation_file
{
	FILE*             file_dst_src;
	const char*       file_name;
	struct stat* 	  info;
	string*           strings;
	size_t            strings_num;
	int               is_initialized;
	compilation_file* binary_file;
	ON_DEBUG(
	unsigned __int64 hash;
	)
	
};
)




ON_DEBUG(

unsigned __int64 MurmurHash(const char* key, size_t data_size);

)

int Ignore_Register_Strcmp(char* cmd, const char* with, size_t cmd_length);

ON_CMP_TEXTLIB_MODE(

void CompilationFileCtor(compilation_file* to_ctor, const char* file_name_to_open);

int IsCompilationFileHashUnchanged(compilation_file* to_check);

void CompilationFileDtor(compilation_file* to_destruct);

)

void MakeStringsFromCompilationFile(compilation_file* source);


void OutputStringsToLog(const string* to_output, size_t num_of_strings);

void ReadToBuffer(FILE*   source, char** ptr_buffer,
				  size_t* get_num_of_strings);  

#endif