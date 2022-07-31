#include "object.h"

Object::Object(Class* _class): type(ObjectType::NonArr), _class(_class), length(0), data(new Slots(_class->instanceSlotCount)) {}
Object::Object(Class* _class, uint8_t type, unsigned int count): type(type), _class(_class), length(count) {
    switch(type){
        case ObjectType::ByteArr:   data = new Byte[count]; 
        case ObjectType::CharArr:   data = new uint16_t[count]; 
        case ObjectType::ShortArr:  data = new short[count]; 
        case ObjectType::IntArr:    data = new int[count];  
        case ObjectType::FloatArr:  data = new float[count];  
        case ObjectType::LongArr:   data = new long[count];
        case ObjectType::DoubleArr: data = new double[count];
        case ObjectType::RefArr:    data = new Ref[count];
    }
}
Object::~Object(){
    switch(type){
        case ObjectType::NonArr:    delete (Slots*)data; break;
        case ObjectType::ByteArr:   delete (Byte*)data; break;
        case ObjectType::CharArr:   delete (uint16_t*)data; break;
        case ObjectType::ShortArr:  delete (short*)data; break;
        case ObjectType::IntArr:    delete (int*)data; break;
        case ObjectType::FloatArr:  delete (float*)data; break;
        case ObjectType::LongArr:   delete (long*)data; break;
        case ObjectType::DoubleArr: delete (double*)data; break;
        case ObjectType::RefArr:    delete (Object*)data; break;
    }
}