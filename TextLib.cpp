
#include "TextLib.h"
#include "D:\ded_projects\Proccessor\log.h"
#include "time.h"



#define READING_PROCCESSING(IGNORE_NEGATIVE_CHAR, IGNORE_SPACE, 				\
						    READ_GOOD_CHAR,       IGNORE_NEWLINE_IN_THE_END) 	\
while ((read_char = fgetc(source)) != EOF)										\
{																				\
	IGNORE_NEGATIVE_CHAR														\
	IGNORE_SPACE																\
	READ_GOOD_CHAR																\
}																				\
IGNORE_NEWLINE_IN_THE_END														\


#define IGNORE_NEGATIVE_CHAR													\
while (read_char <= 0 && read_char != EOF)										\
		read_char = fgetc(source);												\

#define IGNORE_SPACE					 										\
while (isspace(read_char) && read_char != EOF && read_char != '\n') 			\
{																				\
	read_char = fgetc(source);								    				\
	while (read_char <= 0 && read_char != EOF)									\
		read_char = fgetc(source);							    				\
}																				\

#define READ_CORRECT_CHAR														\
while (read_char != '\n' && read_char != EOF)									\
{																				\
	buffer[num_of_read_char] = read_char;										\
	num_of_read_char++;															\
	read_char = fgetc(source);													\
																				\
	if(read_char == '\n') {														\
		num_of_strings++;														\
		buffer[num_of_read_char++] = read_char;									\
	}																			\
}																				\


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
	int read_char           = 0;
	size_t num_of_read_char = 0;
	size_t num_of_strings   = 0;
	char* buffer 			= *ptr_buffer;
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
	
	for(int i = 0; i < num_of_string; ++i)
	{
		while(*end_line != '\n' && *end_line != '\0')
			end_line++;

		str[i].line   = begin_line;

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
/*
int Ignore_Register_Strcmp(char* str1, char* str2, size_t str_length)
{
    for(int cmd_char_count = 0; cmd_char_count < str_length; ++cmd_char_count)
    {
        str1[cmd_char_count] = tolower(str1[cmd_char_count]);
		str2[cmd_char_count] = tolower(str2[cmd_char_count]);
    }
    return strcmp((const char*)cmd, with);
}
*/
///-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/// compilation_file function

static void TransferTimeToFileNameFormat(char* time)
{
	if(time == nullptr) {
		__PRINT_ALL_INFO__("something went wrong, the program doesn't know the time\n");
		//return;
	}

	for(size_t char_in_time = 0; time[char_in_time] != '\0'; ++char_in_time)
	{
		if(time[char_in_time] == '\n' || time[char_in_time] == ':')
			time[char_in_time] = ' ';
	}

}


//ON_CMP_TEXTLIB_MODE(

void CompilationFileCtor(compilation_file* to_ctor, const char* source_name)
{
	compilation_file null_object = {0};

	if(memcmp(&null_object, to_ctor, sizeof(compilation_file)) != 0)
	{
		__PRINT_ALL_INFO__("Inccorect construction: you give unitialized "
						   "or already initialized object\n");
		return;						   
	}

	if(to_ctor == nullptr) {
		__PRINT_ALL_INFO__("Inccorect construction: you give nullptr argument\n");
		return;
	}
	if(source_name == nullptr) {
		__PRINT_ALL_INFO__("You give for function inccorect pointer (name of file) << he is nullptr\n");
		return;
	}
	
	if((to_ctor->file_dst_src = fopen(source_name, "rb")) == nullptr) {
		__PRINT_ALL_INFO__("Something went wrong while opening the file");
		PrintToLog        (" << file name is - %s\n", source_name);
		return;
	}

	struct stat* src_file_info 	= (struct stat*)calloc(1, sizeof(struct stat));
	if(stat(source_name, src_file_info) != 0) {
		__PRINT_ALL_INFO__("Something went wrong, the program can't take info about your file");
		PrintToLog        (" << file name is - %s\n");
		return;
	}
	to_ctor->file_name 		    = source_name;
	to_ctor->info               = src_file_info;
	to_ctor->strings            = nullptr;
	to_ctor->strings_num        = 0;
	to_ctor->is_initialized     = 1;
	struct tm* compilation_time = nullptr;
	time_t current_time         = time(NULL);
	compilation_time		    = localtime(&current_time);

	char* file_name_with_compilation_time = asctime(compilation_time);
	TransferTimeToFileNameFormat(file_name_with_compilation_time);

	const char* file_ready_name   = strcat(file_name_with_compilation_time, source_name);
	struct stat* binary_file_info = (struct stat*)calloc(1, sizeof(struct stat));
	stat(file_ready_name, binary_file_info);

	to_ctor->binary_file        = (compilation_file*)calloc(1, sizeof(compilation_file)); 
	//check
	FILE* open_binary_file      = fopen(file_ready_name, "wb");
	
	if(open_binary_file == nullptr) {
		__PRINT_ALL_INFO__("Something went wrong while opening your binary file\n")
	}
	*to_ctor->binary_file     = { 
								   open_binary_file,						/// file_dst_src
								   file_name_with_compilation_time,			/// file_name
								   binary_file_info,						/// info
								   nullptr,									/// strings
								   0,										/// strings_num
								   1,										/// is_initialized
								   nullptr,									/// binary_file		
								   ON_DEBUG(0)								/// hash
								};

	ON_DEBUG(
		to_ctor->binary_file->hash = MurmurHash((const char*)to_ctor->binary_file, sizeof(compilation_file) - sizeof(__int64));
	to_ctor->hash                  = MurmurHash((const char*)to_ctor, sizeof(compilation_file) - sizeof(__int64));
	)
}

//fflush() see

void MakeStringsFromCompilationFile(compilation_file* source)
{
	if(source == nullptr) {
		__PRINT_ALL_INFO__("Nullptr compilation_file\n");
		return;
	}

	if(ON_DEBUG(!IsCompilationFileHashUnchanged(source) || ) source->is_initialized == 0)
		return;
	
	char*  buffer         = MakeBufferForFile(source->file_dst_src);
	size_t num_of_strings = 0;

	ReadToBuffer(source->file_dst_src, &buffer, &num_of_strings);
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
	if(to_destruct->info == nullptr && to_destruct->binary_file->info == nullptr) {
		__PRINT_ALL_INFO__("Something went wrong, info about your compilation_file is dissapear\n")
		return;
	}
	fclose(to_destruct->binary_file->file_dst_src);
	fclose(to_destruct->file_dst_src);
	free(to_destruct->binary_file);
	free(to_destruct->binary_file->info);
	free(to_destruct->info);
	compilation_file null_object = {0};
	*to_destruct = null_object;
}

//ON_DEBUG(
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
	if(to_check->binary_file->hash != MurmurHash((const char*)to_check->binary_file, sizeof(compilation_file) - sizeof(__int64))) {
		__PRINT_ALL_INFO__("The binary data was corrupted\n")
		return 0;
	}

	return 1;
}

unsigned __int64 MurmurHash(const char* key, size_t data_size)
{
  const unsigned __int64 first_hash_coefficient  = 0x5bd1e995;
  const unsigned __int64 seed                    = 0;
  const int              binary_offset           = 24;

  unsigned __int64       hash                    = seed ^ data_size;

  const unsigned char *  data                    = (const unsigned char *)key;
  unsigned __int64       second_hash_coefficient = 0;

  while (data_size >= 4)
  {
      second_hash_coefficient  = data[0];
      second_hash_coefficient |= data[1] << 8;
      second_hash_coefficient |= data[2] << 16;
      second_hash_coefficient |= data[3] << 24;

      second_hash_coefficient *= first_hash_coefficient;
      second_hash_coefficient ^= second_hash_coefficient >> binary_offset;
      second_hash_coefficient *= first_hash_coefficient;

      second_hash_coefficient *= first_hash_coefficient;
      hash                    ^= second_hash_coefficient;


      data += 4;
      data_size -= 4;
  }

  switch (data_size)
  {
    case 3:
      hash ^= data[2] << 16;
    case 2:
      hash ^= data[1] << 8;
    case 1:
      hash ^= data[0];
      hash *= first_hash_coefficient;
  };

  hash ^= hash >> 13;
  hash *= first_hash_coefficient;
  hash ^= hash >> 15;

  return hash;
}

//)
//)

#undef IGNORE_SPACE
#undef IGNORE_NEWLINE_IN_THE_END
#undef READ_CORRECT_CHAR
#undef READING_PROCCESSING
#undef IGNORE_NEGATIVE_CHAR