#ifndef MATH_INSTRUCTION_GUARD
#define MATH_INSTRUCTION_GUARD
#include "instruction.h"
#include <iostream>
#include <cmath>

// binary operation method
template<typename T> struct UnaryOperandInstrucion : public NoOperandsInstruction{
    typedef T(*Op)(T);
    Op op;
    UnaryOperandInstrucion(Op op): op(op){}
    void execute(Frame* frame){
        T v = frame->pop<T>(); 
        frame->push(op(v));
    }
};
template<typename T> T _NEG(T v){ return -v; }

// binary operation method
template<typename T> struct BinaryOperandInstrucion : public NoOperandsInstruction{
    typedef T(*Op)(T, T);
    Op op;
    BinaryOperandInstrucion(Op op): op(op){}
    void execute(Frame* frame){
        T v2 = frame->pop<T>(); 
        T v1 = frame->pop<T>(); 
        frame->push(op(v1, v2));
    }
};
template<typename T> T _REM(T v1, T v2){ 
    if(v2 == 0){
        std::cerr << "java.lang.ArithmeticException: / by zero" << std::endl;
        exit(1);
    } 
    return v1 % v2; 
}
template<> inline  float _REM(float v1, float v2){ return fmod(v1, v2); }
template<> inline double _REM(double v1, double v2){ return fmod(v1, v2); }
template<typename T, bool IS_RIGHT, bool IS_SIGNED = true> T _SHIFT(T v1, T v2){
    unsigned int bitCount =  log2(sizeof(T) * 8);
    unsigned int mask = (1U << bitCount) - 1;
    uint32_t s = (uint32_t)v2 & mask;
    if(!IS_SIGNED){
        if(sizeof(T) == 32){
            v1 = (unsigned int)v1;
        }
        else{
            v1 = (unsigned long)v1;
        }
    }
    if(IS_RIGHT){
        return v1 >> s;
    }
    else{   
        return v1 << s;
    }    
} 
template<typename T> T _AND(T v1, T v2){ return v1 & v2; }
template<typename T> T _OR(T v1, T v2){ return v1 | v2; }
template<typename T> T _XOR(T v1, T v2){ return v1 ^ v2; }
template<typename T> T _ADD(T v1, T v2){ return v1 + v2; }
template<typename T> T _SUB(T v1, T v2){ return v1 - v2; }
template<typename T> T _MUL(T v1, T v2){ return v1 * v2; }
template<typename T> T _DIV(T v1, T v2){ 
    if(v2 == 0){
        std::cerr << "java.lang.ArithmeticException: / by zero" << std::endl;
        exit(1);
    } 
    return v1 / v2; 
}

// NEG
template<typename T> struct NEG : public UnaryOperandInstrucion<T>{ NEG():UnaryOperandInstrucion<T>(_NEG<T>){}};
typedef NEG<double> DNEG;
typedef NEG<float>  FNEG;
typedef NEG<int>    INEG;
typedef NEG<long>   LNEG;

// REM
template<typename T> struct REM : public BinaryOperandInstrucion<T>{ REM():BinaryOperandInstrucion<T>(_REM<T>){}};
typedef REM<double> DREM;
typedef REM<float>  FREM;
typedef REM<int>    IREM;
typedef REM<long>   LREM;

// SHIFT
template<typename T, bool IS_RIGHT, bool IS_SIGNED = true>
struct SHIFT : public BinaryOperandInstrucion<T>{ SHIFT(): BinaryOperandInstrucion<T>(_SHIFT<T, IS_RIGHT, IS_SIGNED>){} };
typedef SHIFT< int, false>        ISHL;
typedef SHIFT< int,  true>        ISHR;
typedef SHIFT< int,  true, false> IUSHR;
typedef SHIFT<long, false>        LSHL;
typedef SHIFT<long,  true>        LSHR;
typedef SHIFT<long,  true, false> LUSHR;

// AND
template<typename T>
struct AND : public BinaryOperandInstrucion<T>{ AND(): BinaryOperandInstrucion<T>(_AND<T>){} };
typedef AND<int>  IAND;
typedef AND<long> LAND;

// OR
template<typename T>
struct OR : public BinaryOperandInstrucion<T>{ OR(): BinaryOperandInstrucion<T>(_OR<T>){} };
typedef OR<int>  IOR;
typedef OR<long> LOR;

// XOR
template<typename T>
struct XOR : public BinaryOperandInstrucion<T>{ XOR(): BinaryOperandInstrucion<T>(_XOR<T>){} };
typedef XOR<int>  IXOR;
typedef XOR<long> LXOR;

// ADD
template<typename T>
struct ADD : public BinaryOperandInstrucion<T>{ ADD(): BinaryOperandInstrucion<T>(_ADD<T>){} };
typedef ADD<int>    IADD;
typedef ADD<long>   LADD;
typedef ADD<float>  FADD;
typedef ADD<double> DADD;

// SUB
template<typename T>
struct SUB : public BinaryOperandInstrucion<T>{ SUB(): BinaryOperandInstrucion<T>(_SUB<T>){} };
typedef SUB<int>    ISUB;
typedef SUB<long>   LSUB;
typedef SUB<float>  FSUB;
typedef SUB<double> DSUB;

// MUL
template<typename T>
struct MUL : public BinaryOperandInstrucion<T>{ MUL(): BinaryOperandInstrucion<T>(_MUL<T>){} };
typedef MUL<int>    IMUL;
typedef MUL<long>   LMUL;
typedef MUL<float>  FMUL;
typedef MUL<double> DMUL;

// DIV
template<typename T>
struct DIV : public BinaryOperandInstrucion<T>{ DIV(): BinaryOperandInstrucion<T>(_DIV<T>){} };
typedef DIV<int>    IDIV;
typedef DIV<long>   LDIV;
typedef DIV<float>  FDIV;
typedef DIV<double> DDIV;

// Increment local variable by constant
struct IINC : public Instruction{
    unsigned int index;
    int constant;
    void fetchOperands(BytecodeReader& br){
        index = (unsigned int)br.readUint8();
        constant = (int)br.readInt8();
    }
    void execute(Frame* frame){
        int var = frame->localVars.getInt(index);
        var += constant;
        frame->localVars.set(index, var);
    }
};
#endif