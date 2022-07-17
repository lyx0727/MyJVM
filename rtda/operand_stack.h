#ifndef OPERAND_STACK_GUARD
#define OPERAND_STACK_GUARD
#include <stack>
#include "slot.h"

class OperandStack{
private:
    size_t maxSize;
    std::stack<Slot> slots;
public:
    OperandStack(size_t maxSize): maxSize(maxSize){}
  
    void push(int val){ slots.push(Slot(val)); }
    void push(float val){ push((int)val); }
    void push(long val){ push((int)val); push((int)(val >> 32)); }
    void push(double val){ push((long)val); }
    void push(Object* ref){ slots.push(Slot(ref)); }
  
    int popInt(){
        int val = slots.top().num;
        slots.pop();
        return val;
    }
    float popFloat(){ return (float)popInt(); }
    long popLong(){ return (long)popInt() << 32 | popInt(); }
    double popDouble(){ return (double)popLong(); }
    Object* popRef(){
        Object* ref = slots.top().ref;
        slots.pop();
        return ref;
    }
};


#endif