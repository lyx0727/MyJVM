#ifndef THREAD_GUARD
#define THREAD_GUARD
#include <cstdint>
#include "jvm_stack.h"
#include "../heap/class.h"

struct Frame;
struct Stack;
struct Object;

class Thread{
private:
    int pc;
    Stack* stack;
public:
    Thread(unsigned int maxSize = 1024);
    ~Thread();

    unsigned int size() const;
    bool empty() const;
    void clear();

    int getPc(){ return pc; }
    void setPc(int pc){ this->pc = pc; }
    void pushFrame(Frame* frame);
    Frame* newFrame(Method* method);
    Frame* popFrame();
    Frame* getCurrentFrame();

    // exception
    bool findAndGotoExceptionHandler(Object* ex);
    void handleUncaughtException(Object* ex);
};

#endif