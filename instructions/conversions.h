#ifndef CONVERSIONS_INSTRUCTION_GUARD
#define CONVERSIONS_INSTRUCTION_GUARD
#include "instruction.h"

// struct D2F : public NoOperandsInstruction{};
struct D2I : public NoOperandsInstruction{
    void execute(Frame* frame){
        double d = frame->operandStack.popDouble(); 
        frame->operandStack.push(toInt(d));
    }
};
// struct D2L : public NoOperandsInstruction{};

#endif