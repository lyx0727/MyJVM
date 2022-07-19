#ifndef FRAME_GUARD
#define FRAME_GUARD
#include "thread.h"
#include "local_vars.h"
#include "operand_stack.h"

class Thread;

struct Frame{
    Frame* lower;
    LocalVars localVars;
    OperandStack operandStack;
    Thread* thread;
    int nextPc;

    Frame(Thread* thread, unsigned int maxLocals, unsigned int maxStack)
        : lower(nullptr), localVars(maxLocals), operandStack(maxStack), thread(thread) {}
};

#endif