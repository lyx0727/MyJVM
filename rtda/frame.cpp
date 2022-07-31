#include "frame.h"
#include "../heap/sym_ref.h"
using namespace std;

ConstantPool* Frame::getConstantPool() const{ return method->_class->constantPool; }
ClassLoader* Frame::getClassLoader() const{ return method->_class->classLoader; }

Frame::Frame(Thread* thread, Method* method)
        : lower(nullptr), localVars(method->maxLocals), operandStack(method->maxStack), thread(thread), nextPc(0), method(method) {}

void Frame::revertNextPC(){ nextPc = thread->getPc(); }

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
    // hack
    if(method->isNative()){
        if(method->name == "registerNatives"){
            thread->popFrame();
        }
        else{
            cerr << "native method: " << method->_class->name << "." << method->name << method->descriptor << endl;
            exit(1);
        }
    }
}

void Frame::initClass(Class* _class){
    // set 'initStarted' true to avoid dead loop
    _class->startInit();
    scheduleClinit(_class);
    initSuperClass(_class);
    
}
void Frame::scheduleClinit(Class* _class){
    Method* clinit = _class->getClintMethod();
    if(clinit != nullptr){
        // exec <cinit>
        Frame* newFrame = thread->newFrame(clinit);
        thread->pushFrame(newFrame);
    }
}  
void Frame::initSuperClass(Class* _class){
    if(!_class->isInterface()){
        Class* superClass = _class->superClass;
        if(superClass != nullptr && !superClass->initStarted){
            initClass(superClass);
        }
    }
}
