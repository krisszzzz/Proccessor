
DEF_CMD(1,      push, 1,  StackPush_int(&to_implement->stack, (const int*)(to_implement->code + ip + 1)); )

DEF_CMD(2,      pop,  0,  StackPop_int (&to_implement->stack); )

DEF_CMD(3,      in,   0,  int in = 0;
                          while(scanf("%d", &in) != 1) {printf("Inccorect input\n");}
                          StackPush_int(&to_implement->stack, &in);
)

DEF_CMD(4,      out,  0,  printf("%d\n", StackTop_int(&to_implement->stack)); )

DEF_CMD(5,      mul,  0,  int mulr =  StackPop_int(&to_implement->stack) *
                                      StackPop_int(&to_implement->stack);
                                      StackPush_int(&to_implement->stack, &mulr); 
)

DEF_CMD(6,      add,  0,  int addr =  StackPop_int(&to_implement->stack) +
                                      StackPop_int(&to_implement->stack);
                                      StackPush_int(&to_implement->stack, &addr);
)

DEF_CMD(7,      sub,  0,  int sub1 = StackPop_int(&to_implement->stack);
                          int sub2 = StackPop_int(&to_implement->stack); 
                          int subr = sub1 - sub2;
                          StackPush_int(&to_implement->stack, &subr);
)

DEF_CMD(8,      div,  0,  int div1 = StackPop_int(&to_implement->stack);                        // divided two 
                          int div2 = StackPop_int(&to_implement->stack);
                          int divr = 0;
                          if(div2 != 0) {
                             divr = div1 / div2;
                          }
                          StackPush_int(&to_implement->stack, &divr);
)
DEF_CMD(9,     draw,  0,  for(int height = 0; height < Height; ++height)                           /// Draw one frame (75 * 200 symbols) to stdount
                          { 
                             fwrite(VideoMemory + height * Width, sizeof(char), Width, stdout);
                             printf("\n");
                          }
                          printf("\n");
                          gotoxy(0, 0);
                          for(int iter_count = 0; iter_count < Height * Width; ++iter_count)
                          {
                             VideoMemory[iter_count] = ' ';
                          }
                          )
DEF_CMD(10,    jb,  1,  
                         int first_val  = StackPop_int(&to_implement->stack);
                         int second_val = StackPop_int(&to_implement->stack);
                         if(first_val < second_val) {
                            ip = *(int*)(to_implement->code + ip + 1);
                            additional_ip_offset = -1 - 1 * 4;
                         } 
)                          

DEF_CMD(11,    call, 1, int next_ip = ip + 4 + 1;
                        StackPush_int(&to_implement->stack, &next_ip);
                        ip = *(int*)(to_implement->code + ip + 1);
                        additional_ip_offset = -1 - 1 * 4;
                        )

DEF_CMD(12,    ret,  0, ip = StackPop_int(&to_implement->stack);
                        additional_ip_offset = -1;
)                       

DEF_CMD(13,    hlt,  0, exit(1000 - 7);)


// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// for register and memory variant of push/pop command

DEF_CMD(1 | 0x70, push, 1, if(*(int*) (to_implement->code + ip + 1) >= 1000 - 7) {
                              int vmem_address            = *(int*) (to_implement->code + ip + 1) - 993;
                              VideoMemory[vmem_address]   = *(char*)(to_implement->code + ip + 5);
                              additional_ip_offset        = 1;                                           // ip have to be increased by one because we also need 
                           } else {
                              StackPush_int(&to_implement->stack, &RAM[*(int*) (to_implement->code + ip + 1)]);
                           }

                        )

DEF_CMD(2 | 0x70, pop,  0,  if(*(int*) (to_implement->code + ip + 1) >= 1000 - 7) {
                           __PRINT_ALL_INFO__("Inccorect address, you can't use video memory with pop function\n");
                            } else {
                               RAM[*(int*)(to_implement->code + ip + 1)] = StackPop_int(&to_implement->stack);
                               additional_ip_offset = 4;                                              //sleep
                            }
)                        

DEF_CMD(1 | 0x60, push, 0, int reg_number = (*(char*)(to_implement->code + ip + 1)) | 0x0;
                           StackPush_int(&to_implement->stack, &Registers[reg_number - 1]);
                           additional_ip_offset = 1; )

DEF_CMD(2 | 0x60, pop,  0, int reg_number = (*(char*)(to_implement->code + ip + 1)) | 0x0;
                           Registers[reg_number - 1] = StackPop_int(&to_implement->stack);
                           additional_ip_offset = 1; ) // sleep
                        
#undef DEF_CMD