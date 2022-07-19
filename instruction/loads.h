#ifndef LOADS_INSTANCE_GUARD
#define LOADS_INSTANCE_GUARD
#include "instruction.h"

// load int from local variable
void _iload(Frame* frame, unsigned int index);

struct ILOAD : public Index8Instruction{ void execute(Frame* frame){ _iload(frame, index); } };
struct ILOAD_0 : public NoOperandsInstruction{ void execute(Frame* frame){ _iload(frame, 0); } };
struct ILOAD_1 : public NoOperandsInstruction{ void execute(Frame* frame){ _iload(frame, 1); } };
struct ILOAD_2 : public NoOperandsInstruction{ void execute(Frame* frame){ _iload(frame, 2); } };
struct ILOAD_3 : public NoOperandsInstruction{ void execute(Frame* frame){ _iload(frame, 3); } };

#endif