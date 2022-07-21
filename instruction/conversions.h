#ifndef CONVERSIONS_INSTRUCTION_GUARD
#define CONVERSIONS_INSTRUCTION_GUARD
#include "instruction.h"

// X -> Y
template<typename X, typename Y>
struct X2Y : public NoOperandsInstruction{
    void execute(Frame* frame){
        X x = frame->pop<X>();
        frame->push(to<Y>(x));
    }
};

// int -> Y
typedef X2Y<int, float>    I2F;
typedef X2Y<int, double>   I2D;
typedef X2Y<int, long>     I2L;
// long -> Y
typedef X2Y<long, float>   L2F;
typedef X2Y<long, double>  L2D;
typedef X2Y<long, int>     L2I;
// float -> Y
typedef X2Y<float, double> F2D;
typedef X2Y<float, int>    F2I;
typedef X2Y<float, long>   F2L;
// double -> Y
typedef X2Y<double, float> D2F;
typedef X2Y<double, int>   D2I;
typedef X2Y<double, long>  D2L;

#endif