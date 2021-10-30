
DEF_CMD(1,      push, 1,  StackPush_int(&to_implement->stack, (const int*)(to_implement->code + ip + 1)); )

DEF_CMD(-127,   push, 1,  int vmem_address            = *(int*) (to_implement->code + ip + 1) - 993;
                          VideoMemory[vmem_address]   = *(char*)(to_implement->code + ip + 5);
                          ip+=1;
                        )
// This dump not will work

DEF_CMD(2,      pop,  1,  StackPop_int (&to_implement->stack); )
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

DEF_CMD(8,      div,  0,  int div1 = StackPop_int(&to_implement->stack);
                          int div2 = StackPop_int(&to_implement->stack);
                          int divr = 0;
                          if(div2 != 0) {
                             divr = div1 / div2;
                          }
                          StackPush_int(&to_implement->stack, &divr);
)
DEF_CMD(9,     draw,  0,  for(int height = 0; height < Height; ++height)
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
//DEF_CMD(16, meow, 0)

#undef DEF_CMD