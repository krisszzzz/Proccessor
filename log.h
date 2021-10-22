#ifndef LOG_INCLUDED
#define LOG_INCLUDED

#define __PRINT_ALL_INFO__(error_message) 																    \
PrintToLog("Error occured in file: %s, function: %s, line: %d\n", __FILE__, __PRETTY_FUNCTION__, __LINE__); \
PrintToLog(error_message);

void RememberLogFile(FILE* log_file);

void ForgetLogFile();

int PrintToLog(const char* format, ...);

#endif
