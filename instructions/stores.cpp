#include "stores.h"
using namespace std;

void _lstore(Frame* frame, unsigned int index){
    long var = frame->operandStack.popLong();
    frame->localvals.set(index, var);
}