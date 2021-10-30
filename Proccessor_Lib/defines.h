#if defined DEBUG
#define ON_DEBUG(code) code
#else
#define ON_DEBUG(code) 
#endif

#if defined WINDOWS													
#define ONLY_ON_WINDOWS(code) code
#else
#define ONLY_ON_WINDOWS(code)  
#endif