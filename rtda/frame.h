#ifndef FRAME_GUARD
#define FRAME_GUARD
#include "local_vars.h"
#include "operand_stack.h"

struct Frame{
    Frame* lower;
    LocalVars localvals;
    OperandStack operandStack;
};

#endif