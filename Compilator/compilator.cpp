#include <string.h>
#include <stdlib.h>
#include "Compilator.h"
#include "..\\Proccessor_lib\\murmurhash.h"
#include "..\\Proccessor_Lib\\binfilename.h"
#undef binary_file


/// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/// For graphics
const char All_pixel_types[]   = ".:;~=#OB8";
const int  All_pixel_types_num = 9; 

static int DefinePixelTypeCode(const char pixel_type)
{
    for(int pixel_code = 0; pixel_code < All_pixel_types_num; pixel_code++)
    {
        if(pixel_type == All_pixel_types[pixel_code])
            return pixel_code;
    }
    return -1;
}

/// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/// checkers


#define DEF_CMD(cmd_code, ...)                        \
case cmd_code: ;

static void CollisionChecker()
{
    const int check_code_collision = 0;

    switch(check_code_collision) {
    #include "..\\Proccessor_Lib\\commands.h"
    }
}
#define REG_CHECK(arg, reg)                           \
if(strncmp(arg, #reg, 2) == 0)						  \
return 1;

static int RegisterChecker(const char* arg)
{
	REG_CHECK(arg, ax);
	REG_CHECK(arg, bx);
	REG_CHECK(arg, cx);
	REG_CHECK(arg, dx);

	return 0;
}

/// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/// compilation_file functions

int CompilationFileCtor(compilation_file* to_ctor, const char* source_name)
{
	compilation_file null_object = {0};

	if(memcmp(&null_object, to_ctor, sizeof(compilation_file)) != 0)
	{
		__PRINT_ALL_INFO__("Inccorect construction: you give unitialized "
						   "or already initialized object\n");
		return ERROR_CONSTRUCTION;						   
	}

	if(to_ctor == nullptr) {
		__PRINT_ALL_INFO__("Inccorect construction: you give nullptr argument\n");
		return ERROR_CONSTRUCTION;
	}
	if(source_name == nullptr) {
		__PRINT_ALL_INFO__("You give for function inccorect pointer (name of file) << he is nullptr\n");
		return ERROR_CONSTRUCTION;
	}
	
	if((to_ctor->src_file = fopen(source_name, "rb")) == nullptr) {
		__PRINT_ALL_INFO__("Something went wrong while opening the file");
		PrintToLog        (" << file name is - %s\n", source_name);
		return ERROR_CONSTRUCTION;
	}

	struct stat src_file_info = {};
	if(stat(source_name, &src_file_info) != 0) {
		__PRINT_ALL_INFO__("Something went wrong, the program can't take info about your file");
		PrintToLog        (" << file name is - %s\n");
		return ERROR_CONSTRUCTION;
	}
	to_ctor->file_name 		      = source_name;
	to_ctor->info                 = src_file_info;
	to_ctor->strings              = nullptr;
	to_ctor->strings_num          = 0;
	to_ctor->is_initialized       = 1;
	char* file_ready_name	 	  = MakeBinFileName(source_name);
	struct stat binary_file_info  = {};
	stat(file_ready_name, &binary_file_info);

	if(to_ctor->info.st_mtime < binary_file_info.st_mtime)	{
		PrintToLog("Already compilated file: file name --> %s\n", source_name);
		PrintToLog("source modification time: %ld, last compilation time: %ld\n", to_ctor->info.st_mtime,
					binary_file_info.st_mtime);
		return ALREADY_COMPILATED;
	}

	FILE* open_binary_file        = fopen(file_ready_name, "wb");
	to_ctor->binary_file		  = calloc(1, sizeof(binary_file)); 
	if(to_ctor->binary_file == nullptr) {
		__PRINT_ALL_INFO__("Something went wrong, inccorect memory allocation\n");
	}
	if(open_binary_file == nullptr) {
		__PRINT_ALL_INFO__("Something went wrong while opening your binary file\n")
	}

	(*(binary_file*)to_ctor->binary_file)  = { 
												open_binary_file,						/// bin_file
												binary_file_info,						/// info
												file_ready_name,						/// file_name
												ON_DEBUG(0)								/// hash
											};		
	ON_DEBUG(									
	(*(binary_file*)to_ctor->binary_file).hash = MurmurHash((char*)(to_ctor->binary_file), sizeof(binary_file) - sizeof(__int64));
	to_ctor->hash                 			   = MurmurHash((const char*)to_ctor, sizeof(compilation_file) - sizeof(__int64));
	)
	return 0;
}


void MakeStringsFromCompilationFile(compilation_file* source)
{
	if(source == nullptr) {
		__PRINT_ALL_INFO__("Nullptr compilation_file\n");
		return;
	}

	if(ON_DEBUG(!IsCompilationFileHashUnchanged(source) || ) source->is_initialized == 0)
		return;
	
	char*  buffer         = MakeBufferForFile(source->src_file);
	size_t num_of_strings = 0;

	ReadToBuffer(source->src_file, &buffer, &num_of_strings);
	string* strings = (string*)calloc(num_of_strings, sizeof(string));
	// Check

	MakeStrings(buffer, strings, num_of_strings);

	source->strings 	  = strings;
	source->strings_num   = num_of_strings;
	ON_DEBUG(
		source->hash      = MurmurHash((const char*)source, sizeof(compilation_file) - sizeof(__int64));
	)
}

void CompilationFileDtor(compilation_file* to_destruct)
{
	if(to_destruct == nullptr) {
		__PRINT_ALL_INFO__("Nullptr compilation_file\n")
		return;
	}
	ON_DEBUG(
		if(!IsCompilationFileHashUnchanged(to_destruct))
			return;
	)
	if(to_destruct->is_initialized != 1) {
		__PRINT_ALL_INFO__("Unitialized compilation_file\n")
		return;
	}
	if(to_destruct->binary_file == nullptr) {
		__PRINT_ALL_INFO__("Binary file is nullptr\n")
		return;
	}

	fclose((*(binary_file*)to_destruct->binary_file).bin_file);
	fclose(to_destruct->src_file);
	free(to_destruct->binary_file);
	compilation_file null_object = {0};
	*to_destruct 				 = null_object;
}

ON_DEBUG(
int IsCompilationFileHashUnchanged(compilation_file* to_check)
{
	if(to_check == nullptr) {
		__PRINT_ALL_INFO__("Nullptr compilation_file\n");
		return 0;
	}

	if(to_check->hash != MurmurHash((const char*)to_check, sizeof(compilation_file) - sizeof(__int64))) {
		__PRINT_ALL_INFO__("The hash of compilation_file is changed\n")
		PrintToLog("current hash - %lg, should be - %lg", to_check->hash, 
		    	   MurmurHash((const char*)to_check, sizeof(compilation_file) - sizeof(__int64)));
		return 0;
	}
	if(to_check->binary_file == nullptr) {
		__PRINT_ALL_INFO__("Something went wrong, your binary file is nullptr\n")
		return 0;
	}
	if((*(binary_file*)to_check->binary_file).hash != MurmurHash((char*)to_check->binary_file, sizeof(binary_file) - sizeof(__int64))) {
		__PRINT_ALL_INFO__("The binary data was corrupted\n")
		return 0;
	}

	return 1;
}
)

/// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/// This is function needed to start compilation (assemblation)

int CompileCmd(compilation_file* to_compile)
{
    if(to_compile == nullptr) {
        __PRINT_ALL_INFO__("Inccorect argument - is nullptr\n")
        return COMPILATION_ERROR;
    }
    if(to_compile->is_initialized == 0) {
        __PRINT_ALL_INFO__("Unitialized compilition_file object\n")
        return COMPILATION_ERROR;
    }
    if(to_compile->strings_num == 0 || to_compile->strings == nullptr) {
        __PRINT_ALL_INFO__("You source file is empty, or probably you forgot to use MakeStringsForCompilationFile() function\n");
        return COMPILATION_ERROR;
    }

    ON_DEBUG(
        if(!IsCompilationFileHashUnchanged(to_compile))
            return COMPILATION_ERROR;
    )

    for(size_t iter_count = 0; iter_count < to_compile->strings_num; ++iter_count)
    {
        char*   cmd              = nullptr;
        char    cmd_code         = 0;
        int     command_length   = 0;
        int     arg_one          = 0; // double
		int     arg_two          = 0;
        int     mem_address      = 0;
        int     cmd_arg_num      = 0;
        bool    is_mem_treatment = 0;
		bool    is_reg_treatment = 0;
        int     second_arg_begin = 0;
		char*   reg_name		 = nullptr;

        if(sscanf(to_compile->strings[iter_count].line, "%ms %n[%d]%n", &cmd, 
                  &command_length, &mem_address, &second_arg_begin) == 2) {
            is_mem_treatment = 1;
        } else if(sscanf(to_compile->strings[iter_count].line, "%ms %n%ms", &cmd, &command_length, &reg_name) == 2) {
			if(RegisterChecker(reg_name)) {
			is_reg_treatment = 1;
			}
		} else {
            sscanf(to_compile->strings[iter_count].line, "%ms %n", &cmd, &command_length);
        }

        cmd_code = CompareCommands(cmd, command_length, &cmd_arg_num);

        if(cmd_code != ERROR_CMD) {
			if(is_reg_treatment && cmd_arg_num != 0) {
				
			}

            if(is_mem_treatment && cmd_arg_num != 0) {    /// For video memory and RAM memory treatment

                cmd_code |= 0x80;            // using binary operations, to label the new code, if push used with argument
                fwrite(&cmd_code,    sizeof(char),  1, (*(binary_file*)to_compile->binary_file).bin_file);
                fwrite(&mem_address, sizeof(int),   1, (*(binary_file*)to_compile->binary_file).bin_file);
                if(mem_address >= 1000 - 7) { // Ded Inside
                    char pixel_type  = 0;
                    if(sscanf(to_compile->strings[iter_count].line + second_arg_begin, " %c", &pixel_type) == 1) {
						if(DefinePixelTypeCode(pixel_type) != -1) {
                        fwrite(&pixel_type, sizeof(char), 1, (*(binary_file*)to_compile->binary_file).bin_file);                  
						}
                    } else {
                        char by_default_pixel_type = '.';															        // If you didn't pass argument, or you pass inccorect
                        fwrite(&by_default_pixel_type, sizeof(char), 1, (*(binary_file*)to_compile->binary_file).bin_file); // argument, the argument automatically will be '.'
                    }
                } else if(mem_address < 1000 - 7 && mem_address >= 0) {
					if(sscanf(to_compile->strings[iter_count].line + second_arg_begin, " %d", &arg_two) == 1) {
						fwrite(&arg_two, sizeof(int), 1, (*(binary_file*)to_compile->binary_file).bin_file);
					} else {
						__PRINT_ALL_INFO__("Inccorect argument, when you treat to the memory\n");
					}
				} else {
					__PRINT_ALL_INFO__("Inccorect memory address\n");
					return COMPILATION_ERROR;
				}
                is_mem_treatment = 0;
                continue;
            }
            
            fwrite(&cmd_code,   sizeof(char),  1, (*(binary_file*)to_compile->binary_file).bin_file);

            if(cmd_arg_num) {
                if(sscanf(to_compile->strings[iter_count].line + command_length, "%d", &arg_one) <= 0) {
                    __PRINT_ALL_INFO__("Inccorect argument, your put incorrect argument\n")
                    PrintToLog        ("Line number - %d, command - %s", iter_count + 1, cmd);
                    return COMPILATION_ERROR;
                } else {
                    fwrite(&arg_one, sizeof(int), 1, (*(binary_file*)to_compile->binary_file).bin_file);
                }
            }
        } else {
            __PRINT_ALL_INFO__("Inccorect command, please see the instruction, where all commands showed\n")
            PrintToLog        ("Your input: %s, line number", cmd, iter_count + 1);

            return COMPILATION_ERROR;
        }
        free(cmd);
    }
    return 0;
}

// WRITE AUTO-GENERATOR TO DIFFER THE 1-BYTE DIFFERENCE

#define DEF_CMD(cmd_code, cmd_name, arg_num, ...)       \
if(strncmp(cmd, #cmd_name, cmd_length) == 0) {          \
    *cmd_arg_num = arg_num;                             \
    return cmd_code;                                    \
}                                                       \


char CompareCommands(const char* cmd, const int cmd_length, int* cmd_arg_num)
{
    #include "..\\Proccessor_Lib\\commands.h"

    return ERROR_CMD;
}

