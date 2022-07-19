#ifndef THREAD_GUARD
#define THREAD_GUARD
#include <cstdint>
#include "jvm_stack.h"

struct Frame;
struct Stack;

class Thread{
private:
    int pc;
    Stack* stack;
public:
    Thread(unsigned int maxSize = 1024);
    ~Thread();
    int getPc(){ return pc; }
    void setPc(int pc){ this->pc = pc; }
    void pushFrame(Frame* frame);
    Frame* newFrame(unsigned int maxLocals, unsigned int maxStack);
    Frame* popFrame();
    Frame* getCurrentFrame();
};

#endif