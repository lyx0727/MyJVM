#include "thread.h"
using namespace std;

const string Slots::toString() const {
    string str;
    for(Slot var : slots){
        str += "(" + to_string((unsigned long)(var.ref)) + ")\n";
    }
    return str;
}   

Thread::Thread(unsigned int maxSize):stack(new Stack(maxSize)){}
Thread::~Thread(){ delete stack; }

unsigned int Thread::size() const{
    return stack->size();
}

bool Thread::empty() const { return size() == 0; }

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