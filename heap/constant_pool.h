#ifndef HEAP_CONSTANT_POOL_GUARD
#define HEAP_CONSTANT_POOL_GUARD
#include <string>
#include <cstring>
#include "class.h"

class Class;
struct Field;
struct Method;

namespace ConstantType{
    enum ConstantType{
        Int, Long, Float, Double, Ref, String
    };
}

struct Constant{
private:
    union ConstantValue{
        int i;
        long l;
        float f;
        double d;
        Ref r;
    } val;
public:
    int type;
    Constant(){}
    Constant(int val): type(ConstantType::Int) { this->val.i = val; }
    Constant(long val): type(ConstantType::Long) { this->val.l = val; }
    Constant(float val): type(ConstantType::Float) { this->val.f = val; }
    Constant(double val): type(ConstantType::Double) { this->val.d = val; }
    Constant(Ref val): type(ConstantType::Ref) { this->val.r = val; }
    Constant(const std::string& str): type(ConstantType::String) { 
        char* ref = new char[str.length()];
        std::strcpy(ref, str.c_str());
        val.r = ref; 
    } 
    ~Constant(){
        if(type == ConstantType::String) delete (char*)val.r;
    }
    template<typename T> T getVal() const;
    const std::string toString() const;
};

template<> inline int Constant::getVal() const { return val.i; }
template<> inline long Constant::getVal() const { return val.l; }
template<> inline float Constant::getVal() const { return val.f; }
template<> inline double Constant::getVal() const { return val.d; }
template<> inline Ref Constant::getVal() const { return val.r; }

class ConstantPool{
private:
    std::vector<Constant> consts;
public:
    Class* _class;
    ConstantPool(Class* _class, classfile::ConstantPool& cp);
    unsigned int size() const { return consts.size(); }

    Constant getConstant(unsigned int index) const { return consts[index]; }
};

#endif