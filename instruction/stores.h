#ifndef STORES_INSTANCE_GUARD
#define STORES_INSTANCE_GUARD
#include "instruction.h"

// store long into local variable
void _lstore(Frame* frame, unsigned int index);

struct LSTORE : public Index8Instruction{ void execute(Frame* frame){  _lstore(frame, index); } };
struct LSTORE_0 : public NoOperandsInstruction{ void execute(Frame* frame){  _lstore(frame, 0); } };
struct LSTORE_1 : public NoOperandsInstruction{ void execute(Frame* frame){  _lstore(frame, 1); } };
struct LSTORE_2 : public NoOperandsInstruction{ void execute(Frame* frame){  _lstore(frame, 2); } };
struct LSTORE_3 : public NoOperandsInstruction{ void execute(Frame* frame){  _lstore(frame, 3); } };


#endif