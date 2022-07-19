#ifndef FRAME_GUARD
#define FRAME_GUARD
#include "thread.h"
#include "local_vars.h"
#include "operand_stack.h"

class Thread;

struct Frame{
    Frame* lower;
    LocalVars localvals;
    OperandStack operandStack;
    Thread* thread;
};

#endif