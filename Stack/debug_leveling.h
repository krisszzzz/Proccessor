
/** Debug levels. If DEBUG_LVL = 1, then canary protection is enabled. It is also guaranteed that the pop function returns zero (null object) 
if there are no elements on the stack. I also check whether the stack was constructed twice or not. In both cases, a warning is issued. On   
DEBUG_LVL = 2, in addition to all the above, hash protection works. On DEBUG_LVL = 0, it is only guaranteed that critical errors will be handled
*/

#ifndef DEBUG_LEVELING_ENABLE

#define DEBUG_LEVELING 

#define DEBUG_LVL 2

#define STACK_USE_HASH   0x01
#define STACK_USE_CANARY 0x02
#define STACK_USE_ASSERT

#if DEBUG_LVL == 2
#define ON_DEBUG_LVL_2(code)  code
#define ON_DEBUG_LVL_1(code)  code
#define STACK_PROT (STACK_USE_HASH | STACK_USE_CANARY)
#elif DEBUG_LVL == 1
#define ON_DEBUG_LVL_2(code)
#define ON_DEBUG_LVL_1(code)  code
#define STACK_PROT STACK_USE_CANARY
#else
#undef  STACK_USE_HASH
#undef  STACK_USE_CANARY
#define STACK_PROT 0
#define ON_DEBUG_LVL_2(code)
#define ON_DEBUG_LVL_1(code)
#endif

#if DEBUG_LVL >= 1
#define WARNING_PROCCESSING(warning_code)                          \
Warning_Proccessing_(warning_code, __PRETTY_FUNCTION__, __LINE__, __FILE__)
#endif


#define ERROR_PROCCESSING(error_code)                             \
Error_Proccessing_(error_code, __PRETTY_FUNCTION__, __LINE__, __FILE__)

#endif


