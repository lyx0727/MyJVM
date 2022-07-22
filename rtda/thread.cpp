#include "thread.h"
using namespace std;

Thread::Thread(unsigned int maxSize):stack(new Stack(maxSize)){}
Thread::~Thread(){ delete stack; }

Frame* Thread::newFrame(unsigned int maxLocals, unsigned int maxStack){
    return new Frame(this, maxLocals, maxStack);
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

const string LocalVars::toString() const {
    string str;
    for(Slot var : vars){
        str += "(" + to_string(var.num) + ")\n";
    }
    return str;
}   