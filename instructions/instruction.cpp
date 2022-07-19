#include "loads.h"
#include "stores.h"
#include "comparisons.h"
#include <cmath>
using namespace std;

void _iload(Frame* frame, unsigned int index){
    int var = frame->localvals.getInt(index);
    frame->operandStack.push(var);
}

void _lstore(Frame* frame, unsigned int index){
    long var = frame->operandStack.popLong();
    frame->localvals.set(index, var);
}

int _compare(const float& lhs, const float& rhs, bool gFlag){
    if(isnan(lhs) || isnan(rhs)){
        return gFlag ? 1 : -1;
    }
    if(lhs > rhs){
        return 1;
    }
    else if(lhs == rhs){
        return 0;
    }
    else{
        return -1;
    }
}

void branch(Frame* frame, int offset){
    int pc = frame->thread->getPc();
    int nextPc = pc + offset;
    frame->thread->setPc(nextPc);
}