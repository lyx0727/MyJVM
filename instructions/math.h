#ifndef MATH_INSTRUCTION_GUARD
#define MATH_INSTRUCTION_GUARD
#include "instruction.h"
#include <iostream>

// struct DREM : public NoOperandsInstruction{};
// struct FREM : public NoOperandsInstruction{};
struct IREM : public NoOperandsInstruction{
    void execute(Frame* frame){
        int v2 = frame->operandStack.popInt(); 
        int v1 = frame->operandStack.popInt();
        if(v2 == 0){
            std::cerr << "java.lang.ArithmeticException: / by zero" << std::endl;
            exit(1);
        } 
        frame->operandStack.push(v1 % v2);
    }
};
// struct LREM : public NoOperandsInstruction{};

struct ISHL : public NoOperandsInstruction{
    void execute(Frame* frame){
        int v2 = frame->operandStack.popInt(); 
        int v1 = frame->operandStack.popInt();
        // low 5 bit
        uint32_t s = (uint32_t)v2 & 0x1f;
        frame->operandStack.push(v1 << s);
    }
};
// struct ISHR : public NoOperandsInstruction{};
// struct IUSHR : public NoOperandsInstruction{};
// struct LSHL : public NoOperandsInstruction{};
// struct LSHR : public NoOperandsInstruction{};
// struct LUSHR : public NoOperandsInstruction{};

struct IAND : public NoOperandsInstruction{
    void execute(Frame* frame){
        int v2 = frame->operandStack.popInt(); 
        int v1 = frame->operandStack.popInt();
        frame->operandStack.push(v1 & v2);
    }
};
// struct LAND : public NoOperandsInstruction{};

// Increment local variable by constant
struct IINC : public Instruction{
    unsigned int index;
    int constant;
    void fetchOperands(BytecodeReader& br){
        index = (unsigned int)br.readUint8();
        constant = (int)br.readInt8();
    }
    void execute(Frame* frame){
        int var = frame->localvals.getInt(index);
        var += constant;
        frame->localvals.set(index, var);
    }
}
#endif