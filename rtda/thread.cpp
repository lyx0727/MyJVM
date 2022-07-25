#include "thread.h"
using namespace std;

Thread::Thread(unsigned int maxSize):stack(new Stack(maxSize)){}
Thread::~Thread(){ delete stack; }

Frame* Thread::newFrame(Method* method){
    return new Frame(this, method);
}
void Thread::pushFrame(Frame* frame){
    stack->push(frame);
}   
Frame* Thread::popFrame(){
    return stack->pop();
}
Frame* Thread::getCurrentFrame(){
    return stack->top();
}

void Frame::branch(int offset){ nextPc = thread->getPc() + offset; }             

Frame::Frame(Thread* thread, Method* method)
        : lower(nullptr), localVars(method->maxLocals), operandStack(method->maxStack), thread(thread), nextPc(0), method(method) {}

const string Slots::toString() const {
    string str;
    for(Slot var : slots){
        str += "(" + to_string((unsigned long)(var.ref)) + ")\n";
    }
    return str;
}   

ConstantPool* Frame::getConstantPool() const{ return method->_class->constantPool; }