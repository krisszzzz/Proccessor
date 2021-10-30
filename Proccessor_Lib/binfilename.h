#include <string.h>

char* MakeBinFileName(const char* source_name)
{
    char* file_ready_name 		  = strdup(source_name);
	char* point                   = strchr(file_ready_name, '.'); /// Finding the point, to correct the format
	*point                        = '_';
	file_ready_name	 			  = strcat(file_ready_name, ".bin");
    return file_ready_name;
}