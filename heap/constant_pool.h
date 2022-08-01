#ifndef HEAP_CONSTANT_POOL_GUARD
#define HEAP_CONSTANT_POOL_GUARD
#include <string>
#include <cstring>
#include "class.h"
#include "sym_ref.h"

class Class;
struct Field;
struct Method;
class ClassRef;
struct FieldRef;
struct MethodRef;
struct InterfaceMethodRef;

namespace ConstantType{
    enum ConstantType{
        Int, Long, Float, Double, String, ClassRef, FieldRef, MethodRef, InterfaceMethodRef, Null
    };
}

class ConstantPool;

struct Constant{
private:
    struct ConstantValue{
        int i;
        long l;
        float f;
        double d;
        Ref r;
    } val;
public:
    int type;
    Constant(): type(ConstantType::Null) { this->val.r = nullptr; }
    Constant(int val): type(ConstantType::Int) { this->val.i = val; }
    Constant(long val): type(ConstantType::Long) { this->val.l = val; }
    Constant(float val): type(ConstantType::Float) { this->val.f = val; }
    Constant(double val): type(ConstantType::Double) { this->val.d = val; }
    Constant(ClassRef* val): type(ConstantType::ClassRef) { this->val.r = val; }
    Constant(MethodRef* val): type(ConstantType::MethodRef) { this->val.r = val; }
    Constant(FieldRef* val): type(ConstantType::FieldRef) { this->val.r = val; }
    Constant(InterfaceMethodRef* val): type(ConstantType::InterfaceMethodRef) { this->val.r = val; }
    Constant(const std::string& str): type(ConstantType::String) { 
        char* s = new char[str.length()];
        std::strcpy(s, str.c_str());
        this->val.r = s;
    } 
    
    friend ConstantPool;
    template<typename T> T getVal() const;
    const std::string toString() const;
};

template<> inline int Constant::getVal() const { return val.i; }
template<> inline long Constant::getVal() const { return val.l; }
template<> inline float Constant::getVal() const { return val.f; }
template<> inline double Constant::getVal() const { return val.d; }
template<> inline Ref Constant::getVal() const { return val.r; }
template<> inline std::string Constant::getVal() const { return std::string((char*)val.r); }
template<> inline ClassRef* Constant::getVal() const { return (ClassRef*)val.r; }
template<> inline MethodRef* Constant::getVal() const { return (MethodRef*)val.r; }
template<> inline FieldRef* Constant::getVal() const { return (FieldRef*)val.r; }
template<> inline InterfaceMethodRef* Constant::getVal() const { return (InterfaceMethodRef*)val.r; }

class ConstantPool{
private:
    std::vector<Constant> consts;
public:
    Class* _class;
    ConstantPool(Class* _class, classfile::ConstantPool& cp);
    ~ConstantPool();

    unsigned int size() const { return consts.size(); }

    Constant& getConstant(unsigned int index) { return consts[index]; }
};

#endif