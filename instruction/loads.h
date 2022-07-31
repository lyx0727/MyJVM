#ifndef LOADS_INSTANCE_GUARD
#define LOADS_INSTANCE_GUARD
#include "instruction.h"
#include "../utils/utils.h"

template<typename T, unsigned int INDEX>
struct LOAD_N : public NoOperandsInstruction{
    void execute(Frame* frame){
        frame->load<T>(INDEX);
    }
};

template<typename T>
struct LOAD : public Index8Instruction{
    void execute(Frame* frame){
        frame->load<T>(index);
    }
};

// int
typedef LOAD<int>          ILOAD;
typedef LOAD_N<int, 0U>    ILOAD_0;
typedef LOAD_N<int, 1U>    ILOAD_1;
typedef LOAD_N<int, 2U>    ILOAD_2;
typedef LOAD_N<int, 3U>    ILOAD_3;
// float
typedef LOAD<float>        FLOAD;
typedef LOAD_N<float, 0U>  FLOAD_0;
typedef LOAD_N<float, 1U>  FLOAD_1;
typedef LOAD_N<float, 2U>  FLOAD_2;
typedef LOAD_N<float, 3U>  FLOAD_3;
// long
typedef LOAD<long>         LLOAD;
typedef LOAD_N<long, 0U>   LLOAD_0;
typedef LOAD_N<long, 1U>   LLOAD_1;
typedef LOAD_N<long, 2U>   LLOAD_2;
typedef LOAD_N<long, 3U>   LLOAD_3;
// double
typedef LOAD<double>       DLOAD;
typedef LOAD_N<double, 0U> DLOAD_0;
typedef LOAD_N<double, 1U> DLOAD_1;
typedef LOAD_N<double, 2U> DLOAD_2;
typedef LOAD_N<double, 3U> DLOAD_3;
// ref
typedef LOAD<Ref>          ALOAD;
typedef LOAD_N<Ref, 0U>    ALOAD_0;
typedef LOAD_N<Ref, 1U>    ALOAD_1;
typedef LOAD_N<Ref, 2U>    ALOAD_2;
typedef LOAD_N<Ref, 3U>    ALOAD_3;

template<typename T> struct TALOAD : public NoOperandsInstruction {
    void execute(Frame* frame){
        int index = frame->pop<int>();
        Ref ref = frame->pop<Ref>();
        if(ref == nullptr){
            throw JavaLangNullPointerException(ref, __FILE__, __LINE__);
        }
        Object* arrRef = (Object*)ref;
        int arrLen = arrRef->length;
        if(index < 0 || index >= arrLen){
            throw JavaLangArrayIndexOutOfBoundsException(index, __FILE__, __LINE__);
        }
        T* arr = (T*)arrRef->data;
        T val = arr[index];
        frame->push(val);
    }
};

typedef TALOAD<Ref>      AALOAD;
typedef TALOAD<Byte>     BALOAD;
typedef TALOAD<uint16_t> CALOAD;
typedef TALOAD<double>   DALOAD;
typedef TALOAD<float>    FALOAD;
typedef TALOAD<int>      IALOAD;
typedef TALOAD<long>     LALOAD;
typedef TALOAD<short>    SALOAD;

#endif