#ifndef CONPARISONS_INSTRUCTION_GUARD
#define CONPARISONS_INSTRUCTION_GUARD
#include <cmath>
#include "instruction.h"

// IF<OP> or IF_CMP<T, OP>
constexpr int EQ /* equal */        = 1;
constexpr int LT /* less than */    = 1 << 1;
constexpr int GT /* greater than */ = 1 << 2;
constexpr int NE /* not equal */    = LT | GT;
constexpr int LE /* less than */    = LT | EQ;
constexpr int GE /* greater than */ = GT | EQ;

// v1 < v2 : -1
// v1 = v2 :  0
// v1 > v2 :  1
template<typename T> int compare(T v1, T v2, bool gFlag = false){
    if(v1 < v2){
        return -1;
    }
    else if(v1 == v2){
        return 0;
    }
    else {
        return 1;
    }
}
template<int OP> bool compare(int flag){
    int op = 0;
    switch (flag){
        case  0: op |= EQ; break; 
        case -1: op |= LT; break; 
        case  1: op |= GT; break; 
    }      
    return (OP & op) != 0;
}
// float or double can be NAN
inline int compare(double v1, double v2, bool gFlag){
    if(std::isnan(v1) || std::isnan(v2)){
        return gFlag ? 1 : -1;
    }
    if(v1 > v2){
        return 1;
    }
    else if(v1 == v2){
        return 0;
    }
    else{
        return -1;
    }
}
inline int compare(float v1, float v2, bool gFlag){ return compare((double)v1, (double)v2, gFlag); }
// Ref only has = or !=
inline int compare(Ref v1, Ref v2){ if(v1 == v2) return 0; else return 1; }

// Compare T
template<typename T, bool G_FLAG = false> struct CMP : public NoOperandsInstruction{
    void execute(Frame* frame){
        T v2 = frame->pop<T>(); 
        T v1 = frame->pop<T>();
        frame->push(compare(v1, v2, G_FLAG));
    }
};
typedef CMP<int>           ICMP;
typedef CMP<long>          LCMP;
typedef CMP<float, false>  FCMPL;
typedef CMP<float, true>   FCMPG;
typedef CMP<double, false> DCMPL;
typedef CMP<double, false> DCMPG;

template<int OP> 
struct IF : public BranchInstruction{
    void execute(Frame* frame){
        int flag = frame->pop<int>();
        if(compare<OP>(flag)){
            frame->branch(offset);
        }
    }
};
typedef IF<EQ> IFEQ;
typedef IF<NE> IFNE;
typedef IF<LT> IFLT;
typedef IF<LE> IFLE;
typedef IF<GT> IFGT;
typedef IF<GE> IFGE;

template<typename T, int OP> struct IF_CMP : public BranchInstruction{
    void execute(Frame* frame){
        T v2 = frame->pop<T>();
        T v1 = frame->pop<T>();
        int flag = compare(v1, v2);
        if(compare<OP>(flag)){
            frame->branch(offset);
        }
    }
};
// int
typedef IF_CMP<int, EQ> IF_ICMPEQ;
typedef IF_CMP<int, NE> IF_ICMPNE;
typedef IF_CMP<int, LT> IF_ICMPLT;
typedef IF_CMP<int, GT> IF_ICMPGT;
typedef IF_CMP<int, LE> IF_ICMPLE;
typedef IF_CMP<int, GE> IF_ICMPGE;
// Ref
typedef IF_CMP<Ref, EQ> IF_ACMPEQ;
typedef IF_CMP<Ref, NE> IF_ACMPNE;

#endif