#ifndef LOG_INCLUDED
#define LOG_INCLUDED
#include <stdio.h>

#define __PRINT_ALL_INFO__(error_message) 																    \
PrintToLog("Error occured in file: %s, function: %s, line: %d\n", __FILE__, __PRETTY_FUNCTION__, __LINE__); \
PrintToLog(error_message);                                                                                  


void SetLogFile(FILE* log_file = nullptr);

void ResetLogFile();

void ResetAllLogFiles();

int PrintToLog(const char* format, ...);

#endif
