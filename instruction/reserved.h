#ifndef INSTRUCTION_RESERVED_GUARD
#define INSTRUCTION_RESERVED_GUARD
#include "instruction.h"
#include "../native/registry.h"

struct INVOKE_NATIVE : public NoOperandsInstruction {
    void execute(Frame* frame){
        Method* method = frame->method;
        NativeMethod nativeMethod = findNative(method->_class->name, method->name, method->descriptor);
        if(nativeMethod == nullptr){
            throw JavaLangUnsatisfiedLinkError(method->_class->name + "." + method->name + method->descriptor, __FILE__, __LINE__);
        }
        nativeMethod(frame);
    }
};

#endif