ON_DEBUG(
unsigned __int64 MurmurHash(const char* key, size_t data_size)
{
  const unsigned __int64 first_hash_coefficient  = 0x5bd1e995;
  const unsigned __int64 seed                    = 0;
  const int              binary_offset           = 24;

  unsigned __int64       hash                    = seed ^ data_size;

  const unsigned char *  data                    = (const unsigned char *)key;
  unsigned __int64       second_hash_coefficient = 0;

  while (data_size >= 4)
  {
      second_hash_coefficient  = data[0];
      second_hash_coefficient |= data[1] << 8;
      second_hash_coefficient |= data[2] << 16;
      second_hash_coefficient |= data[3] << 24;

      second_hash_coefficient *= first_hash_coefficient;
      second_hash_coefficient ^= second_hash_coefficient >> binary_offset;
      second_hash_coefficient *= first_hash_coefficient;

      second_hash_coefficient *= first_hash_coefficient;
      hash                    ^= second_hash_coefficient;


      data += 4;
      data_size -= 4;
  }

  switch (data_size)
  {
    case 3:
      hash ^= data[2] << 16;
    case 2:
      hash ^= data[1] << 8;
    case 1:
      hash ^= data[0];
      hash *= first_hash_coefficient;
  };

  hash ^= hash >> 13;
  hash *= first_hash_coefficient;
  hash ^= hash >> 15;

  return hash;
}
)