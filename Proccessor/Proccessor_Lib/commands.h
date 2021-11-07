///  cmd_code cmd_name with_double_arg with_mem_treatment with_label_treatment with_reg_treatment code_to_input
///     |        |    +------+               |                     |                  |                |
///     |        |    |  +-------------------+                     |                  |                |
///     |        |    |  |  +--------------------------------------+                  |                |
///     |        |    |  |  |  +------------------------------------------------------+                |
///     V        V    V  V  V  V                                                                       V
DEF_CMD(1,      push, 1, 0, 0, 0,  StackPush_double(&to_implement->stack, (const double*)(to_implement->code + ip + 1)); )
                                                                                        /// divided by 4 because of int* pointer arithmetic
DEF_CMD(2,      pop,  0, 0, 0, 0,  StackPop_double (&to_implement->stack); )

DEF_CMD(3,      in,   0, 0, 0, 0,  double in = 0;
                                   while(scanf("%lf", &in) != 1) {printf("Inccorect input\n");
                                                                  }
                                   StackPush_double(&to_implement->stack, &in);
)

 
DEF_CMD(4,      out,  0, 0, 0, 0,  printf("%lf\n", StackTop_double(&to_implement->stack)); )


DEF_CMD(5,      mul,  0, 0, 0, 0,  double mulr =  StackPop_double(&to_implement->stack) *
                                   StackPop_double(&to_implement->stack);
                                   StackPush_double(&to_implement->stack, &mulr); 
)

DEF_CMD(6,      add,  0, 0, 0, 0,  double addr =  StackPop_double(&to_implement->stack) +
                                   StackPop_double(&to_implement->stack);
                                   StackPush_double(&to_implement->stack, &addr);
)

DEF_CMD(7,      sub,  0, 0, 0, 0,  double sub1 = StackPop_double(&to_implement->stack);
                                   double sub2 = StackPop_double(&to_implement->stack); 
                                   double subr = sub1 - sub2;
                                   StackPush_double(&to_implement->stack, &subr);
)

DEF_CMD(8,      div,  0, 0, 0, 0,  double div1 = StackPop_double(&to_implement->stack);                        // divided two 
                                   double div2 = StackPop_double(&to_implement->stack);
                                   double divr = 0;
                                   if(!IsEqual(div2, 0)) {
                                       divr = div1 / div2;
                                   }
                                   StackPush_double(&to_implement->stack, &divr);
)
DEF_CMD(9,     draw,  0, 0, 0, 0, 
                                   for(int height = 0; height < SCREEN_HEIGHT_IN_ASCII_SYMBOLS; ++height)                           /// Draw one frame (75 * 200 symbols) to stdount
                                   { 
                                       fwrite(proccessor->VideoMemory + height * SCREEN_WIDTH_IN_ASCII_SYMBOLS, 
                                              sizeof(char), SCREEN_WIDTH_IN_ASCII_SYMBOLS, stdout);
                                       printf("\n");
                                   }
                                   printf("\n");
                                   gotoxy(0, 0);
                                   for(int iter_count = 0; iter_count < SCREEN_HEIGHT_IN_ASCII_SYMBOLS * SCREEN_WIDTH_IN_ASCII_SYMBOLS; ++iter_count)
                                   {
                                       proccessor->VideoMemory[iter_count] = ' ';
                                   }
)

DEF_CMD(10,    hlt,   0, 0, 0, 0,  exit(1000 - 7);)


/// Labels 

DEF_CMD(11,    jb,    0, 0, 1, 0,  
                                   double first_val  = StackPop_double(&to_implement->stack);
                                   double second_val = StackPop_double(&to_implement->stack);
                                   if(first_val < second_val) {
                                       ip = *(to_implement->code + ip + 1);
                                       //printf("%d", ip);
                                       additional_ip_offset =  - 2;
                                   } 
)                          
                                                     
DEF_CMD(12,    call,  0, 0, 1, 0,  double d_next_ip = (int)ip + 2;
                                   StackPush_double(&to_implement->stack, &d_next_ip);
                                   ip = *(to_implement->code + ip + 1);
                                   additional_ip_offset = - 2;
                                   //additional_ip_offset = -1 - 1 * 4;
                        )

DEF_CMD(13,    ret,   0, 0, 0, 0,  double d_ip = StackPop_double(&to_implement->stack);
                                   ip = (int)d_ip;
                                   additional_ip_offset = - 1;
)                       

DEF_CMD(14,   ja,   0, 0, 1, 0, 
                                    double first_val  = StackPop_double(&to_implement->stack);
                                    double second_val = StackPop_double(&to_implement->stack);
                                    if(first_val > second_val) {
                                       ip = *(to_implement->code + ip + 1);
                                       //printf("%d", ip);
                                       additional_ip_offset =  - 2;
                                    } )

DEF_CMD(15,    jmp,  0, 0, 1, 0,    ip = to_implement->code[ip + 1];
                                    additional_ip_offset = -2;)

DEF_CMD(16,    sqrt, 0, 0, 0, 0,    double to_sqrt = StackPop_double(&to_implement->stack);
                                    to_sqrt = sqrt(to_sqrt);
                                    StackPush_double(&to_implement->stack, &to_sqrt); )

DEF_CMD(17,    je,   0, 0, 1, 0,    double first_val  = StackPop_double(&to_implement->stack);
                                    double second_val = StackPop_double(&to_implement->stack);
                                    if(IsEqual(first_val,second_val)) {
                                       ip = *(to_implement->code + ip + 1);
                                       additional_ip_offset =  - 2;
                                   } )                                    


// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// for register and memory variant of push/pop command

DEF_CMD(1 | 0x70, push, 0, 1, 0, 0, if(*(int*) (to_implement->code + ip + 1) >= 1000 - 7) {
                                       int vmem_address                        = *(int*) (to_implement->code + ip + 1) - 993;
                                       proccessor->VideoMemory[vmem_address]   = *(char*)(to_implement->code + ip + 2);
                                       additional_ip_offset                    = 1;      
                                    } else {
                                       double to_push = proccessor->RAM[*(int*)(to_implement->code + ip + 1)];
                                       StackPush_double(&to_implement->stack, &to_push);
                                    }
                        )

DEF_CMD(2 | 0x70, pop,  0, 1, 0, 0,  if(*(int*) (to_implement->code + ip / 4 + 1) >= 1000 - 7) {
                                       __PRINT_ALL_INFO__("Inccorect address, you can't use video memory with pop function\n");
                                    } else {
                                       proccessor->RAM[*(int*)(to_implement->code + ip + 1)] = StackPop_double(&to_implement->stack);
                                    }
)                        

/// Registers

DEF_CMD(1 | 0x60, push, 0, 0, 0, 1, int reg_number = *(to_implement->code + ip + 1);
                                    StackPush_double(&to_implement->stack, &proccessor->Registers[reg_number - 1]);
                                    )

DEF_CMD(2 | 0x60, pop,  0, 0, 0, 1, int reg_number = *(to_implement->code + ip + 1);
                                    proccessor->Registers[reg_number - 1] = StackPop_double(&to_implement->stack);
                                    ) // sleep
                        
#undef DEF_CMD