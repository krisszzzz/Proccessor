#include "Stack.h"


ON_DEBUG_LVL_1(

void Warning_Proccessing_(const int warning_code, const char* function_name,
                                                  const int line, const char* file_name)
{
    PrintToLog("Warning occured in fuction: %s \n line: %d file: %s\n", function_name, line, file_name);
    switch(warning_code)
		{
			case kNotInitializedStack:  {
			    PrintToLog("Warning description: Not initialized stack, your stack has been"
                             " automatically zeroed (only for debug level 1 and higher)\n");
				break;
			}
			case kStackUnderflow:  {
				PrintToLog("Warning description: stack underflow, pop function will"
                             " return you zeroed stack (only on debug level 1 and higher)\n");
				break;
			}
            case kInccorectCapacityInputToCtor:  {
                PrintToLog("Warning description: inccorect capacity input to ctor,"
                             " you passed the wrong capacity to the constructor (negative capacity)\n");
                break;
            }
            case kAlreadyConstructed:  {
                PrintToLog("Warning description: Your stack was already constructed, "
                             "you will lose access to your stack data that was already constructed\n");
                break;
            }
            default:  {
                PrintToLog("Warning code is incorrect, something went wrong\n");
            }

		}
}
)

void Error_Proccessing_(const int error_code, const char* function_name, const int line,
                        const char* file_name)
{

	PrintToLog("Error occured in fuction: %s \n line: %d file: %s\n", function_name, line, file_name);

    switch(error_code)
    {
        case kStackDataNullptr:  {
			    PrintToLog("error description: your stack data is nullptr\n");
                break;
        }
        case kNullptrStack:  {
                PrintToLog("error description: you passed to the function that mentioned before the nullptr-object\n");
                break;
        }
        case kMemoryAllocationError:  {
                PrintToLog("error description: something went wrong in memory allocation or reallocation\n");
                break;
        }
        case kStackDataHashIsInccorect:  {
                PrintToLog("error description: your stack data was corrupted, stack data hash was changed\n");
                break;
        }
        case kStackDataLeftCanaryIsInccorect:  {
                PrintToLog("error description: stack left canary (for data) has been changed\n");
                break;
        }
        case kStackDataRightCanaryIsInccorect:  {
                PrintToLog("error description: stack right canary (for data) has been changed\n");
                break;
        }
        case kStackHashIsInccorect:  {
                PrintToLog("error description: your stack was corrupted, stack hash was changed\n");
                break;
        }
        case kStackSizeIsNegative: {
                PrintToLog("error description: stack size can't be negative\n");
                break;
        }
        case kStackSizeBiggerThanCapacity:  {
                PrintToLog("error description: stack size can't be bigger than capacity\n");
                break;
        }
        case kStackRightCanaryIsInccorect:  {
                PrintToLog("error description: your stack right canary has been changed\n");
                break;
        }
        case kStackLeftCanaryIsInccorect:  {
                PrintToLog("error description: your stack left canary has been changed\n");
                break;
        }
        default:  {
                PrintToLog("Error code is incorrect, something went wrong\n");
                break;
        }
    }
    #ifdef STACK_USE_ASSERT
    assert(!"Error see the log file\n");
    #endif
}
int Max(const int first, const int second)
{
    return (first <= second) ? second : first;
}

void* recalloc(void* block, size_t elem_count, size_t elem_size)
{
	return realloc(block, elem_count * elem_size);
}

ON_DEBUG_LVL_1(

void SetDataCanary(char* data, size_t size_of_data)
{
    *(__int64*)data = kLeftCanaryValue;
    data += size_of_data + kCanarySize;
    *(__int64*)data = kRightCanaryValue;
}

void ResetDataRightCanary(char* data, size_t size_of_data)
{
    data += size_of_data + kCanarySize;
    *(__int64*)data = 0;
}

)

ON_DEBUG_LVL_2(

unsigned __int64 CalculateDataHash(char* data, size_t data_size)
{
    return MurmurHash((char*)data, data_size + STACK_USE_CANARY * kCanarySize);
}



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

/*
unsigned __int64 RSHash(const char* test, size_t obj_size)                     
{                                                                               
    unsigned __int64 a = 63689;
    unsigned __int64 b = 378551;
    unsigned __int64 hash = 0;

    for (int i = 0; i < obj_size; ++i)
    {
        hash = hash * a + (unsigned __int64)test[i];
        a *= b;
    }

    return hash;
}
*/

)

