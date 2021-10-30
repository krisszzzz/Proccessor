#include "compilator.h"
//#include "compilator.cpp"               //REMOVE
//#include "cmptextlib.cpp"               //
//#include "textlib.cpp"                  //
//#include "..\proccessor_lib\log.cpp"    // Check collision of symbol with cmd code

int main(int argc, char* argv[])
{
    if(argc == 1) {
        __PRINT_ALL_INFO__("Inccorect input -- please input in cmd the file path and name\n");
        return -1;
    }
    //FILE* log                = fopen("log.txt", "w");
    //RememberLogFile(log);
    compilation_file source  = {0};

    if(CompilationFileCtor(&source, argv[1]) == ALREADY_COMPILATED)
        return 0;
        
    MakeStringsFromCompilationFile(&source);
    CompileCmd(&source);
    CompilationFileDtor(&source);
}

