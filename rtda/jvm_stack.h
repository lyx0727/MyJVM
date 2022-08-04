#ifndef JVM_STACK_GUARD
#define JVM_STACK_GUARD
#include "frame.h"

struct Frame;

class Stack{
private:
    unsigned int maxSize;
    unsigned int sz;
    Frame* _top;
public:
    Stack(unsigned int maxSize): maxSize(maxSize), sz(0U), _top(nullptr){}
    ~Stack();
    
    void push(Frame* frame);
    unsigned int size() const { return sz; }
    bool empty() const { return size() == 0; }
    void clear();

    Frame* top();
    Frame* pop();
};

#endif