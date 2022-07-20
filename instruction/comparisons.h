#ifndef CONPARISONS_INSTRUCTION_GUARD
#define CONPARISONS_INSTRUCTION_GUARD
#include <cmath>
#include "instruction.h"

template<typename T> int compare( T v1, T v2, bool gFlag){
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

inline int compare(float v1, float v2, bool gFlag){
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

// Compare T
template<typename T, bool G_FLAG = false> struct CMP : public NoOperandsInstruction{
    void execute(Frame* frame){
        T v2 = frame->pop<T>(); 
        T v1 = frame->pop<T>();
        frame->push(compare(v1, v2, G_FLAG));
    }
};
typedef CMP<int>          ICMP;
typedef CMP<long>         LCMP;
typedef CMP<float, true>  FCMPG;
typedef CMP<float, false> FCMPL;

// IF<OP>
constexpr int EQ = 1;
constexpr int LT = 1 << 1;
constexpr int GT = 1 << 2;
constexpr int NE = LT | GT;
constexpr int LE = LT | EQ;
constexpr int GE = GT | EQ;

template<int OP> 
struct IF : public BranchInstruction{
    void execute(Frame* frame){
        int var = frame->pop<int>();
        int op = 0;
        switch (var){
            case  0: op |= EQ; break; 
            case -1: op |= LT; break; 
            case  1: op |= GT; break; 
        }
        if((op & OP) != 0){
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

// struct IF_ICMPEQ : public BranchInstruction{};
struct IF_ICMPNE : public BranchInstruction{
    void execute(Frame* frame){
        int v2 = frame->operandStack.popInt();
        int v1 = frame->operandStack.popInt();
        if(v1 != v2){
            branch(frame, offset);
        } 
    }
};
// struct IF_ICMPLT : public BranchInstruction{};
// struct IF_ICMPLE : public BranchInstruction{};
// struct IF_ICMPGT : public BranchInstruction{};
// struct IF_ICMPGE : public BranchInstruction{};
struct IF_ACMPEQ : public BranchInstruction{
    void execute(Frame* frame){
        Ref ref2 = frame->operandStack.popRef();
        Ref ref1 = frame->operandStack.popRef();
        if(ref1 == ref2){
            branch(frame, offset);
        } 
    }
};
// struct IF_ACMPNE : public BranchInstruction{};

#endif