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

    // load T from local variable
    template<typename T> void load(unsigned int index){
        T var = localVars.get<T>(index);
        operandStack.push(var);
    }
    // store T into local variable
    template<typename T> void store(unsigned int index){
        T var = operandStack.pop<T>();
        localVars.set(index, var);
    }
    template<typename T> void push(T var){ operandStack.push(var); }
    template<typename T> T pop(){ return operandStack.pop<T>(); }
    // jump to brand statement
    void branch(int offset);
};

#endif