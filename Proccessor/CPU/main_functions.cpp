#include "main_functions.h"

void printer(const double* to_output)
{
    if(to_output != nullptr) {
        PrintToLog("%lf", *to_output);
    } else {
        __PRINT_ALL_INFO__("Error: nullptr double\n");    
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

int MainArgProccessing(int argc, char* argv[], char** bin_file_name)
{
    switch(argc) 
    {
        case 1: {
            __PRINT_ALL_INFO__("Inccorect cmd argument, you should add argument <-- -C <path to file, that should be compilated> "
                               "or if you already have compiled file write the path to your file\n");
            return -1;
        }
        case 2: {
            *bin_file_name = argv[1];
            return 0;
        }
        
        case 3: {
            if(strncmp(argv[1], "-A", 2) != 0 && strncmp(argv[1], "-D", 2) != 0) {
                __PRINT_ALL_INFO__("Inccorect argument, it should be -A (to Assemble) or -D (to Disassemble)\n");
                return -1;
            }

            char* src_file = nullptr;
            *bin_file_name  = strdup(argv[2]);
            if(strncmp(argv[1], "-D", 2) == 0) {
                system("g++ ../proccessor_lib/log.cpp ../disasm/disasm.cpp ../disasm/main.cpp -o dasm.exe");
                src_file        = strdup("dasm.exe ");
                *bin_file_name  = strcat(*bin_file_name, ".bin");
            }
            if(strncmp(argv[1], "-A", 2) == 0) {
                system("g++ ../proccessor_lib/log.cpp ../proccessor_lib/textlib.cpp ../asm/asm.cpp ../asm/main.cpp ../proccessor_lib/murmurhash.cpp "
                       "-o asm.exe");
                src_file        = strdup("asm.exe ");
                *bin_file_name  = strcat(*bin_file_name, ".bin");
                
            }
            
            src_file            = strcat(src_file, argv[2]);
            system(src_file);
                
            return 0;
        }
        default: {
            __PRINT_ALL_INFO__("Inccorect argument amount\n");
            return -1;
        }    
    }
}
