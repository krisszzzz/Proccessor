#include "textlib.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#define WINDOWS
#include "defines.h"

#define READING_PROCCESSING(_IGNORE_NEGATIVE_CHAR, _IGNORE_SPACE, 				\
						    _READ_GOOD_CHAR,       _IGNORE_NEWLINE_IN_THE_END) 	\
while ((read_char = (char)fgetc(source)) != EOF)								\
{																				\
	IGNORE_NEGATIVE_CHAR														\
	IGNORE_SPACE																\
	READ_CORRECT_CHAR															\
}																				\
IGNORE_NEWLINE_IN_THE_END														


#define IGNORE_NEGATIVE_CHAR													\
while (read_char <= 0 && read_char != EOF)										\
		read_char = (char)fgetc(source);												

#define IGNORE_SPACE					 										\
while (isspace(read_char) && read_char != EOF && read_char != '\n') 			\
{																				\
	read_char = (char)fgetc(source);								    		\
	while (read_char <= 0 && read_char != EOF)									\
		read_char = (char)fgetc(source);							    		\
}																				

#define READ_CORRECT_CHAR														\
while (read_char != '\n' && read_char != EOF)									\
{																				\
	buffer[num_of_read_char] = read_char;										\
	num_of_read_char++;															\
	read_char = (char)fgetc(source);											\
																				\
	if(read_char == '\n') {														\
		num_of_strings++;														\
		buffer[num_of_read_char++] = read_char;									\
	}																			\
}																				


#define IGNORE_NEWLINE_IN_THE_END												\
if (buffer[num_of_read_char - 1] == '\n') {										\
	num_of_strings--;															\
	buffer[num_of_read_char - 1] = '\0';										\
	if(buffer[num_of_read_char - 2] == '\r')									\
		buffer[num_of_read_char - 2] = '\0';									\
}

char* MakeBufferForFile(FILE* source)
{
    fseek(source, 0, SEEK_END);
	size_t file_size = ftell(source);
	fseek(source, 0, SEEK_SET);

	char* buffer = (char*)calloc(file_size, sizeof(char));
	return buffer;
}

void ReadToBuffer(FILE*   source, char** ptr_buffer,
				  size_t* get_num_of_strings)
{
	if(source == nullptr) {
		__PRINT_ALL_INFO__("Error description: FILE* - pointer to file is nullptr\n");
	}
	char   read_char        = 0;
	size_t num_of_read_char = 0;
	size_t num_of_strings   = 0;
	char*  buffer 			= *ptr_buffer;
	READING_PROCCESSING(
		IGNORE_NEGATIVE_CHAR,
		IGNORE_SPACE, 
		READ_CORRECT_CHAR,
		IGNORE_NEWLINE_IN_THE_END
	)

	buffer = (char*)realloc((void*)buffer, (num_of_read_char + 1) * sizeof(char)); /// We allocated one more byte special for 
	buffer[num_of_read_char] = '\0';											   /// '\0' symbol 

	*get_num_of_strings = num_of_strings + 1; /// We increased num_of_strings by 1, because the algoritm count number of '\n'
										      /// and therefore numbers of strings ignore the first line, because '\n' don't stand
											  /// at the begining of new line
}

void MakeStrings(char* buffer, string* str, size_t num_of_string)
{
	char* begin_line = buffer;
	char* end_line   = buffer; 
	
	for(size_t line_num = 0; line_num < num_of_string; ++line_num)
	{
		while(*end_line != '\n' && *end_line != '\0')
			end_line++;

		str[line_num].line  = begin_line;

		ONLY_ON_WINDOWS(
			if(*end_line != '\0')
				*(end_line - 1) = '\0';
		)

		*end_line  = '\0';
		begin_line = ++end_line;
	}
}

void OutputStringsToLog(const string* to_output, size_t num_of_strings)
{
	for(size_t string_count = 0; string_count < num_of_strings; ++string_count)
		PrintToLog("%s\n", to_output[string_count].line);
}
#undef IGNORE_SPACE
#undef IGNORE_NEWLINE_IN_THE_END
#undef READ_CORRECT_CHAR
#undef READING_PROCCESSING
#undef IGNORE_NEGATIVE_CHAR