#ifndef EXTENDED_INSTRUCTION_GUARD
#define EXTENDED_INSTRUCTION_GUARD
#include "instruction.h"

struct WIDE : Instruction{
    Instruction* modifiedInstruction;
    void fetchOperands(BytecodeReader& br){
        
    }
};

#endif