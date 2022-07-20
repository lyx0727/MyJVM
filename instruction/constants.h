#ifndef CONSTANTS_INSTRUCTION_GUARD
#define CONSTANTS_INSTRUCTION_GUARD
#include "instruction.h"

// not operation
struct NOP : public NoOperandsInstruction{
    void execute(Frame* frame){}
};

struct ACONST_NULL : public NoOperandsInstruction{
    void execute(Frame* frame){
        frame->operandStack.push(nullptr);
    }
};

template<typename T, const T& VAL> 
struct CONST : public NoOperandsInstruction{
    void execute(Frame* frame){
        frame->operandStack.push(VAL);
    }
};

constexpr double D_0  = .0;
constexpr double D_1  = 1.0;
constexpr float  F_0  = .0;
constexpr float  F_1  = 1.0;
constexpr float  F_2  = 2.0;
constexpr int    I_0  = 0;
constexpr int    I_1  = 1;
constexpr int    I_2  = 2;
constexpr int    I_3  = 3;
constexpr int    I_4  = 4;
constexpr int    I_5  = 5;
constexpr int    I_M1 = -1;
constexpr long   L_0  = 0L;
constexpr long   L_1  = 1L;
typedef CONST<double, D_0> DCONST_0;
typedef CONST<double, D_1> DCONST_1;
typedef CONST<float,  F_0> FCONST_0;
typedef CONST<float,  F_1> FCONST_1;
typedef CONST<float,  F_2> FCONST_2;
typedef CONST<int,   I_M1> ICONST_M1;
typedef CONST<int,    I_0> ICONST_0;
typedef CONST<int,    I_1> ICONST_1;
typedef CONST<int,    I_2> ICONST_2;
typedef CONST<int,    I_3> ICONST_3;
typedef CONST<int,    I_4> ICONST_4;
typedef CONST<int,    I_5> ICONST_5;
typedef CONST<long,   L_0> LCONST_0;
typedef CONST<long,   L_1> LCONST_1;

struct BIPUSH : public Instruction{ 
    int8_t val; 
    void fetchOperands(BytecodeReader& br){
        val = br.readInt8();
    }
    void execute(Frame* frame){
        frame->operandStack.push((int)val);
    }
};
struct SIPUSH : public Instruction{ 
    int16_t val;
    void fetchOperands(BytecodeReader& br){
        val = br.readInt16();
    }
    void execute(Frame* frame){
        frame->operandStack.push((int)val);
    }
};


#endif