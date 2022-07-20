#ifndef STACKS_INSTRUCTION_GUARD
#define STACKS_INSTRUCTION_GUARD
#include "instruction.h"

// Pop the top operand stack value
struct POP : public NoOperandsInstruction{
    void execute(Frame* frame){
        frame->operandStack.popSlot();
    }
};
// Pop the top one or two operand stack value
struct POP2 : public NoOperandsInstruction{
    void execute(Frame* frame){
        frame->operandStack.popSlot();
        frame->operandStack.popSlot();
    }
};

// Duplicate the top operand stack value
struct DUP : public NoOperandsInstruction{
    void execute(Frame* frame){
        Slot slot = frame->operandStack.popSlot();
        frame->operandStack.push(slot);
        frame->operandStack.push(slot);
    }
};
// Duplicate the top one or two operand stack values
struct DUP2 : public NoOperandsInstruction{
    void execute(Frame* frame){
        Slot slot1 = frame->operandStack.popSlot();
        Slot slot2 = frame->operandStack.popSlot();
        frame->operandStack.push(slot2);
        frame->operandStack.push(slot1);
        frame->operandStack.push(slot2);
        frame->operandStack.push(slot1);
    }
};

// Swap the top two operand stack values
struct SWAP : public NoOperandsInstruction{
    void execute(Frame* frame){
        Slot slot1 = frame->operandStack.popSlot();
        Slot slot2 = frame->operandStack.popSlot();
        frame->operandStack.push(slot1);
        frame->operandStack.push(slot2);
    }
};


#endif