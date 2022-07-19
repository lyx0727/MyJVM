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
    Thread(unsigned int maxSize);
    ~Thread();
    int getPc(){ return pc; }
    void setPc(int pc){ this->pc = pc; }
    void pushFrame(Frame* frame);
    Frame* popFrame();
    Frame* currentFrame();
};

#endif