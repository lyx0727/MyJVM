#ifndef OBJECT_GUARD
#define OBJECT_GUARD
#include "../rtda/slot.h"
#include "class.h"

namespace ObjectType{
    enum ObjectType{
        NonArr, ByteArr, CharArr, ShortArr, IntArr, FloatArr, LongArr, DoubleArr, RefArr 
    };
}

struct Class;

struct Object{
    uint8_t type;
    Class* _class;
    unsigned int length;
    Ref data;
    
    Object(Class* _class);
    Object(Class* _class, uint8_t type, unsigned int count);
    ~Object();
    
    bool isInstanceOf(Class* _class) const;
    Slots* getFields() { return (Slots*)data; }
    Byte* getBytes() { return (Byte*)data; }
    char16_t* getChars() { return (char16_t*)data; }
    short* getShorts() { return (short*)data; }
    int* getInts() { return (int*)data; }
    float* getFloats() { return (float*)data; }
    long* getLongs() { return (long*)data; }
    double* getDoubles() { return (double*)data; }
    Object** getRefs() { return (Object**)data; }

    void setRefVar(const std::string& name, const std::string& descriptor, Object* ref);
    Object* getRefVar(const std::string& name, const std::string& descriptor);
};

#endif