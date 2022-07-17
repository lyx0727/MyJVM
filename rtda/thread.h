#ifndef THREAD_GUARD
#define THREAD_GUARD
#include <cstdint>
#include "stack.h"
#include "frame.h"

class Thread{
private:
    int pc;
    Stack stack;
public:
    Thread(unsigned int maxSize):stack(maxSize){}
    int getPc(){ return pc; }
    void setPc(int pc){ this->pc = pc; }
    void pushFrame(const Frame& frame);
    Frame& popFrame();
    Frame& currentFrame();
};

#endif