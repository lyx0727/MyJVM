#ifndef FRAME_GUARD
#define FRAME_GUARD
#include "thread.h"
#include "local_vars.h"
#include "operand_stack.h"
#include "../heap/class.h"

class Thread;
struct Method;
class ConstantPool;

struct Frame{
    Frame* lower;
    LocalVars localVars;
    OperandStack operandStack;
    Thread* thread;
    int nextPc;
    Method* method;

    Frame(Thread* thread, Method* method);

    ConstantPool* getConstantPool() const;

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
    // operand stack
    template<typename T> void push(T var){ operandStack.push(var); }
    template<typename T> T pop(){ return operandStack.pop<T>(); }
    // local vars
    template<typename T> T get(unsigned int index){ return localVars.get<T>(index); }
    template<typename T> void set(unsigned int index, T val){ return localVars.set(index, val); }
    // jump to brand statement
    void branch(int offset);
    // load constant variable from constant pool 
    void ldc(unsigned int index);
    // load wide constant variable
    void ldc_w(unsigned int index);
    // invoke method
    void invokeMethod(Method* method);
};

#endif