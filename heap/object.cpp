#include "object.h"
using namespace std;

Object::Object(Class* _class)
:type(ObjectType::NonArr), _class(_class), length(0), data(new Slots(_class->instanceSlotCount)), extra(nullptr) {}
Object::Object(Class* _class, uint8_t type, unsigned int count)
:type(type), _class(_class), length(count), extra(nullptr) {
    switch(type){
        case ObjectType::ByteArr:   data = new Byte[count]; break;
        case ObjectType::CharArr:   data = new char16_t[count + 1]; ((char16_t*)data)[count] = 0; break;
        case ObjectType::ShortArr:  data = new short[count]; break;
        case ObjectType::IntArr:    data = new int[count];  break;
        case ObjectType::FloatArr:  data = new float[count]; break;
        case ObjectType::LongArr:   data = new long[count]; break;
        case ObjectType::DoubleArr: data = new double[count]; break;
        case ObjectType::RefArr:    data = new Ref[count]; break;
    }
}

void Object::setExtra(StackTraceElements* stes){
    extra = stes;
    type = ObjectType::Throwable;
}
void Object::setExtra(Class* _class){ extra = _class; }

Object::~Object(){
    switch(type){
        case ObjectType::NonArr:    delete (Slots*)data; break;
        case ObjectType::ByteArr:   delete (Byte*)data; break;
        case ObjectType::CharArr:   delete (char16_t*)data; break;
        case ObjectType::ShortArr:  delete (short*)data; break;
        case ObjectType::IntArr:    delete (int*)data; break;
        case ObjectType::FloatArr:  delete (float*)data; break;
        case ObjectType::LongArr:   delete (long*)data; break;
        case ObjectType::DoubleArr: delete (double*)data; break;
        case ObjectType::RefArr:    delete (Object**)data; break;
    }
    // if(extra){
    //     if(type == ObjectType::Throwable){
    //         delete (StackTraceElements*)extra;
    //     }
    // }
}

bool Object::isInstanceOf(Class* _class) const {
    return _class->isAssignableFrom(this->_class);
}

void Object::setRefVar(const string& name, const string& descriptor, Object* ref){
    Field* field = _class->getField(name, descriptor, false);
    Slots* slots = getFields();
    slots->set(field->slotId, ref);
}

Object* Object::getRefVar(const string& name, const string& descriptor){
    Field* field = _class->getField(name, descriptor, false);
    Slots* slots = getFields();
    return (Object*)slots->get<Ref>(field->slotId);
}

const string Object::CString(){
    char16_t* chars = getRefVar("value", "[C")->getChars();
    return utf16_to_utf8(u16string(chars));
}