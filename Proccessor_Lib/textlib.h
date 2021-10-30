#ifndef TEXTLIB_INCLUDED
#define TEXTLIB_INCLUDED

#include "log.h"

struct string
{
	char* line;
};


void  OutputStringsToLog(const string* to_output, 		   size_t  num_of_strings);

void  ReadToBuffer	    (FILE*         source,    		   char**  ptr_buffer,
				  	     size_t*       get_num_of_strings						 );  

char* MakeBufferForFile (FILE* 		   source									 );

void  MakeStrings	    (char* 		   buffer, 	  		   string* str, 
					 	 size_t 	   num_of_string);

#endif