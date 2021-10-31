#include <string.h>
#include <stdlib.h>
#include "asm.h"
#include "..\\Proccessor_lib\\murmurhash.h"
#include "..\\Proccessor_Lib\\binfilename.h"
#undef binary_file

/// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/// Labels
const int Labels_amount = 10;

struct Label
{
	int ip;
	int label;
} Labels[Labels_amount] = {0};

static void SetLabels()
{
	for(int iter_count = 0; iter_count < Labels_amount; ++iter_count)
	{
		Labels[iter_count].label =  -1;
		Labels[iter_count].ip    =  -1;
	}
}

static int AddLabel(int label, int ip)
{
	static int label_index = 0;
	if(label_index >= 10) {
		__PRINT_ALL_INFO__("Error, labels overflow\n");
		return -1;
	} 
	Labels[label_index].label = label;
	Labels[label_index].ip	  = ip;
	++label_index;

	return 0;
}

static int GetLabelIndex(int label)
{
	for(int label_index = 0; label_index < Labels_amount; ++label_index)
	{
		if(Labels[label_index].label == label) {
			return label_index;
		}
	}
	return 0;
}


/// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/// For graphics
const char All_pixel_types[]   = "%.:;~=#OB8";
const int  All_pixel_types_num = 10; 

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
#define REG_CHECK(arg, reg, reg_code)                 \
if(strncmp(arg, #reg, 2) == 0)						  \
return reg_code;

static char RegisterChecker(const char* arg)
{
	REG_CHECK(arg, ax, 1);
	REG_CHECK(arg, bx, 2);
	REG_CHECK(arg, cx, 3);
	REG_CHECK(arg, dx, 4);

	return 0;
}

/// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/// asm_file functions

int AsmFileCtor(asm_file* to_ctor, const char* source_name)
{
	asm_file null_object = {0};

	if(memcmp(&null_object, to_ctor, sizeof(asm_file)) != 0)
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
		PrintToLog("source modification time: %ld, last Asm time: %ld\n", to_ctor->info.st_mtime,
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
	to_ctor->hash                 			   = MurmurHash((const char*)to_ctor, sizeof(asm_file) - sizeof(__int64));
	)
	return 0;
}


void MakeStringsFromAsmFile(asm_file* source)
{
	if(source == nullptr) {
		__PRINT_ALL_INFO__("Nullptr asm_file\n");
		return;
	}

	if(ON_DEBUG(!IsAsmFileHashUnchanged(source) || ) source->is_initialized == 0)
		return;
	
	char*  buffer         = MakeBufferForFile(source->src_file);
	size_t num_of_strings = 0;

	ReadToBuffer(source->src_file, &buffer, &num_of_strings);
	string* strings = (string*)calloc(num_of_strings, sizeof(string));
	if(strings == nullptr) {
		__PRINT_ALL_INFO__("Failed memory allocation\n");
	}

	MakeStrings(buffer, strings, num_of_strings);

	source->strings 	  = strings;
	source->strings_num   = num_of_strings;
	ON_DEBUG(
		source->hash      = MurmurHash((const char*)source, sizeof(asm_file) - sizeof(__int64));
	)
}

void AsmFileDtor(asm_file* to_destruct)
{
	if(to_destruct == nullptr) {
		__PRINT_ALL_INFO__("Nullptr asm_file\n")
		return;
	}
	ON_DEBUG(
		if(!IsAsmFileHashUnchanged(to_destruct))
			return;
	)
	if(to_destruct->is_initialized != 1) {
		__PRINT_ALL_INFO__("Unitialized asm_file\n")
		return;
	}
	if(to_destruct->binary_file == nullptr) {
		__PRINT_ALL_INFO__("Binary file is nullptr\n")
		return;
	}

	fclose((*(binary_file*)to_destruct->binary_file).bin_file);
	fclose(to_destruct->src_file);
	free(to_destruct->binary_file);
	asm_file null_object = {0};
	*to_destruct 		 = null_object;
}

ON_DEBUG(
int IsAsmFileHashUnchanged(asm_file* to_check)
{
	if(to_check == nullptr) {
		__PRINT_ALL_INFO__("Nullptr asm_file\n");
		return 0;
	}

	if(to_check->hash != MurmurHash((const char*)to_check, sizeof(asm_file) - sizeof(__int64))) {
		__PRINT_ALL_INFO__("The hash of asm_file is changed\n")
		PrintToLog("current hash - %lg, should be - %lg", to_check->hash, 
		    	   MurmurHash((const char*)to_check, sizeof(asm_file) - sizeof(__int64)));
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
/// This is function needed to start Asm (assemblation)

int AssembleCmd(asm_file* to_Assemble)
{
	static int num_of_assemblations = 0;
	++num_of_assemblations;

    if(to_Assemble == nullptr) {
        __PRINT_ALL_INFO__("Inccorect argument - is nullptr\n")
        return ASM_ERROR;
    }
    if(to_Assemble->is_initialized == 0) {
        __PRINT_ALL_INFO__("Unitialized compilition_file object\n")
        return ASM_ERROR;
    }
    if(to_Assemble->strings_num == 0 || to_Assemble->strings == nullptr) {
        __PRINT_ALL_INFO__("You source file is empty, or probably you forgot to use MakeStringsForAsmFile() function\n");
        return ASM_ERROR;
    }

    ON_DEBUG(
        if(!IsAsmFileHashUnchanged(to_Assemble))
            return ASM_ERROR;
    )
	int ip = 0;
	if(num_of_assemblations == 1) {
		SetLabels();
	}
	

    for(size_t iter_count = 0; iter_count < to_Assemble->strings_num; ++iter_count)
    {
        char*   cmd                = nullptr;
        char    cmd_code           = 0;
        int     command_length     = 0;
		int     label_length	   = 0;
        int     arg                = 0; // double
        int     mem_address        = 0;
        int     is_cmd_with_arg    = 0;
        bool    is_mem_treatment   = 0;
		bool    is_reg_treatment   = 0;
		bool    is_label_treatment = 0;
        int     second_arg_begin   = 0;
		char    reg_code           = 0;
		char*   reg_name		   = nullptr;
		int     label			   = 0;
		
		/// Check for labels
		if(sscanf(to_Assemble->strings[iter_count].line, ":%d", &label) == 1) {
			if(num_of_assemblations == 1) {
			AddLabel(label, ip);
			}
			continue;
		}
		/// Get cmd
		sscanf(to_Assemble->strings[iter_count].line, "%ms%n", &cmd, &command_length);
		cmd_code = CompareCommands(cmd, command_length, &is_cmd_with_arg);

		/// Check cmd 
		if(cmd_code == ERROR_CMD) {
			__PRINT_ALL_INFO__("Inccorect command, please see the instruction, where all commands showed\n")
            PrintToLog        ("Your input: %s, line number %d\n", cmd, iter_count + 1);

            return ASM_ERROR;
		}

        if(sscanf(to_Assemble->strings[iter_count].line + command_length, " [%d]%n", &mem_address, &second_arg_begin) == 1) { 		
            is_mem_treatment = 1;																	/// Format, when you have to use RAM or Videomemory

        } else if(sscanf(to_Assemble->strings[iter_count].line + command_length, " %ms", &reg_name) == 1 &&
				 (sscanf(to_Assemble->strings[iter_count].line + command_length, " %d",  &arg) == 0  &&
				  sscanf(to_Assemble->strings[iter_count].line + command_length, " :%d", &arg) == 0) ) {	 // remove

			if((reg_code = RegisterChecker(reg_name)) != 0) {										/// Format, when you have to use registers
				is_reg_treatment = 1;

			} else {
				__PRINT_ALL_INFO__("Inccorect register name\n");
				PrintToLog("Line:%d command:%s", iter_count + 1, cmd);
			}
			free(reg_name);

		} else if(sscanf(to_Assemble->strings[iter_count].line + command_length, " :%d", &label) == 1) {  // ja, jae, jb, jbe je, jne, jmp format
			is_label_treatment = 1;
		} 
		/// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		/// Work with labels

		if(is_label_treatment) {
			int index = GetLabelIndex(label);
			if(num_of_assemblations == 2) {
				fwrite(&cmd_code, 		  sizeof(char), 1,  (*(binary_file*)to_Assemble->binary_file).bin_file);
				fwrite(&Labels[index].ip,  sizeof(int), 1,  (*(binary_file*)to_Assemble->binary_file).bin_file);
			}
			ip += sizeof(char) + sizeof(int);
			is_label_treatment = 0;
			continue;
		}
		
			/// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
			/// Working with registers

		if(is_reg_treatment) {
			cmd_code |= 0x60;           // using binary operations, to label the new code, if push/pop used with register treatment
			if(num_of_assemblations == 2) {
				fwrite(&cmd_code,    sizeof(char),  1, (*(binary_file*)to_Assemble->binary_file).bin_file);
				fwrite(&reg_code,    sizeof(char),  1, (*(binary_file*)to_Assemble->binary_file).bin_file);
			}
			ip += 2*sizeof(char);
			is_reg_treatment = 0;
			continue;
		}
			/// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
			/// Working with RAM and Video memory

        if(is_mem_treatment) {    		 /// For video memory and RAM memory treatment
            cmd_code |= 0x70;            // using binary operations, to label the new code, if push/pop used with RAM treatment
			if(num_of_assemblations == 2) {
            	fwrite(&cmd_code,    sizeof(char),  1, (*(binary_file*)to_Assemble->binary_file).bin_file);
			}
			ip += sizeof(char);
			if(mem_address < 0) {
				__PRINT_ALL_INFO__("Inccorect address\n");
				PrintToLog("Line:%d command:%s", iter_count + 1, cmd);
				return ASM_ERROR;
			}
			if(num_of_assemblations == 2) {
            	fwrite(&mem_address, sizeof(int),   1, (*(binary_file*)to_Assemble->binary_file).bin_file);
			}
			ip += sizeof(int);
			if(is_cmd_with_arg && mem_address >= 1000 - 7) {
                								  // Ded Inside

                char pixel_type  = 0;
                if(sscanf(to_Assemble->strings[iter_count].line + command_length + second_arg_begin, " %c", &pixel_type) == 1) {

					if(DefinePixelTypeCode(pixel_type) != -1) {
						if(num_of_assemblations == 2) {
                        	fwrite(&pixel_type, sizeof(char), 1, (*(binary_file*)to_Assemble->binary_file).bin_file);                  
						}
						ip += sizeof(char);
					} else {
						__PRINT_ALL_INFO__("Inccorect video type object\n");
						PrintToLog("Line:%d command:%s", iter_count + 1, cmd);
					}

                } else {
                    char by_default_pixel_type = '.';															        // If you didn't pass argument, or you pass inccorect
					if(num_of_assemblations == 2) {
                    	fwrite(&by_default_pixel_type, sizeof(char), 1, (*(binary_file*)to_Assemble->binary_file).bin_file); // argument, the argument automatically will be '.'
					}
					ip += sizeof(char);
                } 
			}
	
            is_mem_treatment = 0;
            continue;
        }
			/// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
			/// Default (ordinary) commands proccessing
        if(num_of_assemblations == 2) {    
        	fwrite(&cmd_code,   sizeof(char),  1, (*(binary_file*)to_Assemble->binary_file).bin_file);
		}
		ip += sizeof(char);

        if(is_cmd_with_arg) {
            if(sscanf(to_Assemble->strings[iter_count].line + command_length, "%d", &arg) <= 0) {

                __PRINT_ALL_INFO__("Inccorect argument, your put incorrect argument\n")
                PrintToLog        ("Line number - %d, command - %s", iter_count + 1, cmd);
                return ASM_ERROR;

            } else {
				if(num_of_assemblations == 2) {
                	fwrite(&arg, sizeof(int), 1, (*(binary_file*)to_Assemble->binary_file).bin_file);
				}
				ip += sizeof(int);
            }
        }
        free(cmd);
    }

	if(num_of_assemblations == 1) {
		AssembleCmd(to_Assemble);
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

