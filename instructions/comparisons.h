#ifndef CONPARISONS_INSTRUCTION_GUARD
#define CONPARISONS_INSTRUCTION_GUARD
#include "instruction.h"

template<typename T>
int _compare(const T& lhs, const T& rhs){
    if(lhs < rhs){
        return -1;
    }
    else if(lhs == rhs){
        return 0;
    }
    else {
        return 1;
    }
}

int _compare(const float& lhs, const float& rhs, bool gFlag);

// Compare long
struct LCMP : public NoOperandsInstruction{
    void execute(Frame* frame){
        int v2 = frame->operandStack.popInt(); 
        int v1 = frame->operandStack.popInt();
        frame->operandStack.push(_compare(v1, v2));
    }
};

// Compare float
struct FCMPG : public NoOperandsInstruction{
    void execute(Frame* frame){
        int v2 = frame->operandStack.popFloat(); 
        int v1 = frame->operandStack.popFloat();
        frame->operandStack.push(_compare(v1, v2, true));
    }
};
struct FCMPL : public NoOperandsInstruction{
    void execute(Frame* frame){
        int v2 = frame->operandStack.popFloat(); 
        int v1 = frame->operandStack.popFloat();
        frame->operandStack.push(_compare(v1, v2, false));
    }
};

// Branch if int comparison with zero succeeds
struct IFEQ : public BranchInstruction{
    void execute(Frame* frame){
        int val = frame->operandStack.popInt();
        if(val == 0){
            branch(frame, offset);
        } 
    }
};
// struct IFNE : public BranchInstruction{};
// struct IFLT : public BranchInstruction{};
// struct IFLE : public BranchInstruction{};
// struct IFGT : public BranchInstruction{};
// struct IFGE : public BranchInstruction{};

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
        Object* ref2 = frame->operandStack.popRef();
        Object* ref1 = frame->operandStack.popRef();
        if(ref1 == ref2){
            branch(frame, offset);
        } 
    }
};
// struct IF_ACMPNE : public BranchInstruction{};

#endif