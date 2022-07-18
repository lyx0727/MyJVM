#ifndef OPERAND_STACK_GUARD
#define OPERAND_STACK_GUARD
#include <stack>
#include "../utils/utils.h"
#include "slot.h"

class OperandStack{
private:
    size_t maxSize;
    std::stack<Slot> slots;
public:
    OperandStack(size_t maxSize): maxSize(maxSize){}
  
    void push(int val){ slots.push(Slot(val)); }
    void push(float val){ push(toInt(val)); }
    void push(long val){ push((int)val); push((int)(val >> 32)); }
    void push(double val){ push(toLong(val)); }
    void push(Object* ref){ slots.push(Slot(ref)); }
    void push(Slot slot){ slots.push(slot); }
  
    int popInt(){ int val = slots.top().num; slots.pop(); return val; }
    float popFloat(){ return toFloat(popInt()); }
    long popLong(){ return (toLong(popInt()) << 32) | toLong(popInt()); }
    double popDouble(){ return toDouble(popLong()); }
    Object* popRef(){ Object* ref = slots.top().ref; slots.pop(); return ref; }
    Slot popSlot(){ Slot slot = slots.top(); slots.pop(); return slot; }
};


#endif