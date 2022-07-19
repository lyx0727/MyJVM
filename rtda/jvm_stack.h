#ifndef JVM_STACK_GUARD
#define JVM_STACK_GUARD
#include "frame.h"

struct Frame;

class Stack{
private:
    unsigned int maxSize;
    unsigned int size;
    Frame* _top;
public:
    Stack(unsigned int maxSize): maxSize(maxSize){}
    ~Stack();
    void push(Frame* frame);
    Frame* top();
    Frame* pop();
};

#endif