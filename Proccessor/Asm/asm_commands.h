///  cmd_code cmd_name with_double_arg with_mem_treatment with_label_treatment with_reg_treatment 
///     |        |    +------+               |                     |                  |           
///     |        |    |  +-------------------+  +------------------+                  |           
///     |        |    |  |  +-------------------+                                     |           
///     |        |    |  |  |  +------------------------------------------------------+           
///     V        V    V  V  V  V                                                                  
DEF_CMD(1,      push, 1, 1, 0, 1)

DEF_CMD(2,      pop,  0, 1, 0, 1)

DEF_CMD(3,      in,   0, 0, 0, 0)

DEF_CMD(4,      out,  0, 0, 0, 0)

DEF_CMD(5,      mul,  0, 0, 0, 0)

DEF_CMD(6,      add,  0, 0, 0, 0)

DEF_CMD(7,     sub,   0, 0, 0, 0)

DEF_CMD(8,      div,  0, 0, 0, 0)

DEF_CMD(9,     draw,  0, 0, 0, 0)                           /// Draw one frame (75 * 200 symbols) to stdout

DEF_CMD(10,    hlt,   0, 0, 0, 0)

DEF_CMD(11,    jb,    0, 0, 1, 0)                          

DEF_CMD(12,    call,  0, 0, 1, 0)

DEF_CMD(13,    ret,   0, 0, 1, 0)                       

DEF_CMD(14,    ja,    0, 0, 1, 0)

DEF_CMD(15,    jmp,  0, 0, 1, 0)

DEF_CMD(16,    sqrt, 0, 0, 0, 0)

DEF_CMD(17,    je,   0, 0, 1, 0)


// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// for register and memory variant of push/pop command
                        
#undef DEF_CMD