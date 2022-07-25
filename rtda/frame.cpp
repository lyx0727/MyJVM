#include "frame.h"
using namespace std;

ConstantPool* Frame::getConstantPool() const{ return method->_class->constantPool; }

Frame::Frame(Thread* thread, Method* method)
        : lower(nullptr), localVars(method->maxLocals), operandStack(method->maxStack), thread(thread), nextPc(0), method(method) {}

void Frame::branch(int offset){ nextPc = thread->getPc() + offset; }             

void Frame::ldc(unsigned int index){
    ConstantPool* cp = getConstantPool();
    Constant c = cp->getConstant(index);
    switch(c.type){
        case ConstantType::Int: push(c.getVal<int>()); break;
        case ConstantType::Float: push(c.getVal<float>()); break;
        // TODO
        // case ConstantType::String: break;
        // case ConstantType::ClassRef: break;
        default:
            std::cerr << "ldc to do" << std::endl;
            exit(1);
    }
}

void Frame::ldc_w(unsigned int index){
    ConstantPool* cp = getConstantPool();
    Constant c = cp->getConstant(index);
    switch(c.type){
        case ConstantType::Long: push(c.getVal<long>()); break;
        case ConstantType::Double: push(c.getVal<double>()); break;
        default:
            throw JavaLangClassFormatError(to_string(c.type), __FILE__, __LINE__);
    }
}

void Frame::invokeMethod(Method* method){
    Frame* newFrame = thread->newFrame(method);
    thread->pushFrame(newFrame);
    // pass argments
    int argSlotCount = method->argSlotCount;
    if(argSlotCount > 0){
        for(int i = argSlotCount - 1; i >= 0; i--){
            Slot slot = pop<Slot>();
            newFrame->set(i, slot);
        }
    }
}