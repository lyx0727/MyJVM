#ifndef CONSTANTS_INSTRUCTION_GUARD
#define CONSTANTS_INSTRUCTION_GUARD
#include "instruction.h"

// not operation
struct NOP : public NoOperandsInstruction{
    void execute(Frame* frame){}
};

struct ACONST_NULL : public NoOperandsInstruction{
    void execute(Frame* frame){
        frame->operandStack.push(nullptr);
    }
};
// struct DCONST_0 : public NoOperandsInstruction{};
// struct DCONST_1 : public NoOperandsInstruction{};
// struct FCONST_0 : public NoOperandsInstruction{};
// struct FCONST_1 : public NoOperandsInstruction{};
// struct FCONST_2 : public NoOperandsInstruction{};
// struct ICONST_M1 : public NoOperandsInstruction{};
// struct ICONST_0 : public NoOperandsInstruction{};
// struct ICONST_2 : public NoOperandsInstruction{};
// struct ICONST_3 : public NoOperandsInstruction{};
// struct ICONST_4 : public NoOperandsInstruction{};
// struct ICONST_5 : public NoOperandsInstruction{};
// struct LCONST_0 : public NoOperandsInstruction{};
// struct LCONST_1 : public NoOperandsInstruction{};

struct BIPUSH : public Instruction{ 
    int8_t val; 
    void fetchOperands(BytecodeReader& br){
        val = br.readInt8();
    }
    void execute(Frame* frame){
        frame->operandStack.push((int)val);
    }
};
struct SIPUSH : public Instruction{ 
    int16_t val;
    void fetchOperands(BytecodeReader& br){
        val = br.readInt16();
    }
    void execute(Frame* frame){
        frame->operandStack.push((int)val);
    }
};


#endif