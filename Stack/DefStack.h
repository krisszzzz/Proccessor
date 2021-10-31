
#define Def_Stack_T(elem_type)                                                                                 \
struct stack_##elem_type {																					   \
                                                                                                               \
    ON_DEBUG_LVL_1(                                                                                            \
    __int64           left_canary;                                                                             \
    )                                                                                                          \
                                                                                                               \
    elem_type*        data;																				       \
    ssize_t           size;																					   \
    ssize_t           capacity;																				   \
                                                                                                               \
    ON_DEBUG_LVL_1(                                                                                            \
    __int64           right_canary;                                                                            \
    )                                                                                                          \
    ON_DEBUG_LVL_2(                                                                                            \
    unsigned __int64  hash;                                                                                    \
    )                                                                                                          \
};                                                                                                             \
int IsNullptrStack_##elem_type (const stack_##elem_type * stack_t);                                            \
void (*SetPrinter_##elem_type(void (*printer)(const elem_type* to_print) = nullptr))(const elem_type* to_print);\
void GeneralInfoStack_##elem_type (const stack_##elem_type* stack_t,                                           \
                                     void (*printer)(const elem_type* to_print) = nullptr);                    \
int IsInvalidStack_##elem_type(const stack_##elem_type* stack_t);				                               \
void SafeGeneralInfoStack_##elem_type(const stack_##elem_type* stack_t,                                        \
                                      void (*printer)(const elem_type* to_print) = nullptr);                   \
void CtorStack_##elem_type(stack_##elem_type* stack_t, ssize_t capacity = kMinCapacity,					       \
                            void (*printer)(const elem_type* to_print) = nullptr);                             \
void StackPush_##elem_type(stack_##elem_type* stack_t, const elem_type* value);		                           \
int IsEmptyStack_##elem_type(stack_##elem_type* stack_t);										               \
elem_type StackTop_##elem_type(stack_##elem_type* stack_t);                                                    \
elem_type StackPop_##elem_type(stack_##elem_type* stack_t);					                                   \
void DtorStack_##elem_type(stack_##elem_type* stack_t);  						                               
