#ifndef STORES_INSTANCE_GUARD
#define STORES_INSTANCE_GUARD
#include "instruction.h"

// store T into local variable
template<typename T>
struct STORE : public Index8Instruction{
    void execute(Frame* frame){
        frame->store<T>(index);
    }
};

template<typename T, unsigned int INDEX>
struct STORE_N : public NoOperandsInstruction{
    void execute(Frame* frame){
        frame->store<T>(INDEX);
    }
};

// int
typedef STORE<int>          ISTORE;
typedef STORE_N<int, 0U>    ISTORE_0;
typedef STORE_N<int, 1U>    ISTORE_1;
typedef STORE_N<int, 2U>    ISTORE_2;
typedef STORE_N<int, 3U>    ISTORE_3;
// float
typedef STORE<float>        FSTORE;
typedef STORE_N<float, 0U>  FSTORE_0;
typedef STORE_N<float, 1U>  FSTORE_1;
typedef STORE_N<float, 2U>  FSTORE_2;
typedef STORE_N<float, 3U>  FSTORE_3;
// long
typedef STORE<long>         LSTORE;
typedef STORE_N<long, 0U>   LSTORE_0;
typedef STORE_N<long, 1U>   LSTORE_1;
typedef STORE_N<long, 2U>   LSTORE_2;
typedef STORE_N<long, 3U>   LSTORE_3;
// double
typedef STORE<double>       DSTORE;
typedef STORE_N<double, 0U> DSTORE_0;
typedef STORE_N<double, 1U> DSTORE_1;
typedef STORE_N<double, 2U> DSTORE_2;
typedef STORE_N<double, 3U> DSTORE_3;
// ref
typedef STORE<Ref>           ASTORE;
typedef STORE_N<Ref, 0U>     ASTORE_0;
typedef STORE_N<Ref, 1U>     ASTORE_1;
typedef STORE_N<Ref, 2U>     ASTORE_2;
typedef STORE_N<Ref, 3U>     ASTORE_3;

#endif