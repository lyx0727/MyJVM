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

    size_t size() const { return slots.size(); }
    bool empty() const { return size() == 0; }
    void clear();
  
    void push(int val){ slots.push(Slot(val)); }
    void push(short val){ push((int)val); }
    void push(Byte val){ push((int)val); }
    void push(uint16_t val){ push((int)val); }
    void push(float val){ push(to<int>(val)); }
    void push(long val){ push((int)val); push((int)(val >> 32)); }
    void push(double val){ push(to<long>(val)); }
    void push(Ref ref){ slots.push(Slot(ref)); }
    void push(Slot slot){ slots.push(slot); }

    int popInt(){ int val = slots.top().num; slots.pop(); return val; }
    int popShort(){ return (short)popInt(); }
    int popChar(){ return (uint16_t)popInt(); }
    int popByte(){ return (Byte)popInt(); }
    float popFloat(){ return to<float>(popInt()); }
    long popLong(){ return (to<long>(popInt()) << 32) | to<long>(popInt()); }
    double popDouble(){ return to<double>(popLong()); }
    Ref popRef(){ Ref ref = slots.top().ref; slots.pop(); return ref; }
    Slot popSlot(){ Slot slot = slots.top(); slots.pop(); return slot; }

    Ref getRefFromTop(unsigned int n) const {
        std::stack<Slot> slotsCopy(slots);
        for(unsigned int i = 0; i < n; i++){
            slotsCopy.pop();
        }
        return slotsCopy.top().ref;
    }

    template<typename T> T pop();

    const std::string toString() const;
};

template<> inline int OperandStack::pop<int>(){ return popInt(); }
template<> inline short OperandStack::pop<short>(){ return popShort(); }
template<> inline uint16_t OperandStack::pop<uint16_t>(){ return popChar(); }
template<> inline Byte OperandStack::pop<Byte>(){ return popByte(); }
template<> inline float OperandStack::pop<float>(){ return popFloat(); }
template<> inline long OperandStack::pop<long>(){ return popLong(); }
template<> inline double OperandStack::pop<double>(){ return popDouble(); }
template<> inline Ref OperandStack::pop<Ref>(){ return popRef(); }
template<> inline Slot OperandStack::pop<Slot>(){ return popSlot(); }


#endif