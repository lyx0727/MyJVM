#include "loads.h"
using namespace std;

void _iload(Frame* frame, unsigned int index){
    int var = frame->localvals.getInt(index);
    frame->operandStack.push(var);
}