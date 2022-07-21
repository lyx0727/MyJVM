#ifndef EXTENDED_INSTRUCTION_GUARD
#define EXTENDED_INSTRUCTION_GUARD
#include "instruction.h"

// TODO
struct WIDE : Instruction{
    Instruction* modifiedInstruction;
    void fetchOperands(BytecodeReader& br){
        
    }
    void execute(Frame* frame){

    }
};

struct IFNULL : public BranchInstruction{
    void execute(Frame* frame){
        Ref ref = frame->pop<Ref>();
        if(ref == nullptr){
            frame->branch(offset);
        }
    }
};
struct IFNONNULL : BranchInstruction{
    void execute(Frame* frame){
        Ref ref = frame->pop<Ref>();
        if(ref != nullptr){
            frame->branch(offset);
        }
    }
};

// Branch always (wide index)
struct GOTO_W : public Instruction {
    int offset;
    void fetchOperands(BytecodeReader& br){
        offset = (int)br.readInt32();
    }
    void execute(Frame* frame){
        frame->branch(offset);
    }
};


#endif