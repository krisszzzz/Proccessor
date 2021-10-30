//#define MAIN  //DON'T REMOVE
#include "..\\Proccessor_lib\\binfilename.h"
//#include "cpu.h"
#include "..\\Proccessor_Lib\\log.cpp" // REMOVE
//#include "..\\..\\stack\\stack.cpp" // REMOVE
#include <string.h>
#include "cpu.cpp"
//Def_Stack_T(int)
// REMOVE
//#include <string.h> // REMOVE
//USE ON_DEBUG
// REWORK FOR DOUBLE
// REWORK TO WORK WITH POP WITH ARG

void printer(const int* to_output)
{
    if(to_output != nullptr) {
        PrintToLog("%d", *to_output);
    } else {
        __PRINT_ALL_INFO__("Error: nullptr integer\n");    
    }
}
void SetFontSize()
{
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;                   // Width of each character in the font
    cfi.dwFontSize.Y = 10;                   // Height
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy(cfi.FaceName, L"Consolas"); // Choose your font
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}
//REWORK DEBUG SYSTEM

int main(int argc, char* argv[])
{
    
    FILE* cpu_log           = fopen("cpu_log.txt",   "w");
    FILE* stack_log         = fopen("stack_log.txt", "w");
    SetLogFile(stack_log);
    SetFontSize();

    char* bin_file_name     = nullptr;
    cpu_bin_file bin_source = {0};
    
    switch(argc) 
    {
        case 1: {
            __PRINT_ALL_INFO__("Inccorect cmd argument, you should add argument <-- -C <path to file, that should be compilated> "
                               "or if you already have compiled file write the path to your file\n");
            return -1;
        }
        case 2: {
            bin_file_name = argv[1];
            break;
        }
        
        case 3: {
            if(strncmp(argv[1], "-C", 2) != 0) {
                __PRINT_ALL_INFO__("Inccorect cmd argument you should use -C in this case\n");
                return -1;
            }
        //    printf("%s, %s", argv[1], argv[2]);
            //int f = 1 | 0x60;
            //printf("%d\n", f);
            system("gcc ../proccessor_lib/log.cpp ../proccessor_lib/textlib.cpp ../compilator/compilator.cpp ../compilator/main.cpp -o cmp.exe");
            //fprintf(test, "%s", argv[2]);
            char* src_file = strdup("cmp.exe ");
            src_file       = strcat(src_file, argv[2]);
            system(src_file);
            bin_file_name  = MakeBinFileName(argv[2]);
            break;
        }
        default: {
            __PRINT_ALL_INFO__("Inccorect argument amount\n");
            return -1;
        }
    }

    CpuBinFileCtor(&bin_source, bin_file_name, printer);
    //CpuBinFileCtor(&bin_source, "..\\Compilator\\source_txt.bin", printer); // REWORK
    ImplementCmd(&bin_source, cpu_log, stack_log);
    //PrintCmdCode();
}


