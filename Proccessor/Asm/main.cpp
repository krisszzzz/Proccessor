#include "asm.h"
//#include "../proccessor_lib/log.cpp"
//#include "asm.cpp"
//#include "../Proccessor_Lib/textlib.cpp"
//#include "../Proccessor_Lib/murmurhash.cpp"

int main(int argc, char* argv[])
{
    if(argc == 1) {
        __PRINT_ALL_INFO__("Inccorect input -- please input in cmd the file path and name\n");
        return -1;
    }
    //FILE* log                = fopen("log.txt", "w");
    //RememberLogFile(log); 
    asm_file source  = {0};
    if(AsmFileCtor(&source, argv[1]) == ALREADY_ASSEMBLED)
        return 0;
        
    MakeStringsFromAsmFile(&source);
    AssembleCmd(&source);
    AsmFileDtor(&source);
}

