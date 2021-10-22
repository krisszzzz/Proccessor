#include <stdio.h>
#include <string.h>
#include "Compilator.h"
//#include "TextLib.cpp" /// REMOVE!!!
#include <sys/stat.h>
#include "D:\ded_projects\Proccessor\log.h"
//#include "D:\ded_projects\Proccessor\log.cpp" //REMOVE!!!


int main()
{
    FILE* log                = fopen("log.txt", "w");
    RememberLogFile(log);
    
    compilation_file source  = {0};
    CompilationFileCtor(&source, "source.txt");
    MakeStringsFromCompilationFile(&source);
    CompileCmd(&source);
    CompilationFileDtor(&source);
}

