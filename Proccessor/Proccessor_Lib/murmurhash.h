#if !defined MURMURHASH_INCLUDED

#define MURMURHASH_INCLUDED
#include <stdlib.h>

#if defined DEBUG
unsigned __int64 MurmurHash(const char* key, size_t data_size);
#endif

#endif