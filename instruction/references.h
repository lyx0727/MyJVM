#ifndef INSTRUCTION_REFERENCES_GUARD
#define INSTRUCTION_REFERENCES_GUARD
#include <iostream>
#include "instruction.h"
#include "../heap/object.h"
#include "../heap/sym_ref.h"

struct NEW : public Index16Instruction{
    void execute(Frame* frame){
        ConstantPool* cp = frame->method->_class->constantPool;
        ClassRef* classRef =  (ClassRef*)cp->getConstant(index).ref;
        Class* _class = classRef->resolvedClass();
        if(_class->isAbstract() || _class->isInterface()){
            std::cerr << "java.lang.InstantiationError" << std::endl;
            exit(1);
        }  
        Ref ref = (Ref)new Object(_class);
        frame->push(ref); 
    }
};

namespace FIELD_OP{
    enum FIELD_OP{
        PUT, GET 
    };
}

template<int OP, bool IS_STATIC>
struct FIELD : public Index16Instruction{
private:
    template<typename T> void operate(Frame* frame, Slots& slots, unsigned int slotId){
        if(OP == FIELD_OP::PUT){
            slots.set(slotId, frame->pop<T>());
        }
        else if(OP == FIELD_OP::GET){
            frame->push(slots.get<T>(slotId));
        }
    }
public:
    void execute(Frame* frame){
        Method* currentMethod = frame->method;
        Class* currentClass = currentMethod->_class;
        ConstantPool* cp = currentClass->constantPool;
        FieldRef* fieldRef = (FieldRef*)cp->getConstant(index).ref;
        Field* field = fieldRef->resolvedFeild();
        if(field->isStatic() != IS_STATIC){
            std::cerr << "java.lang.IncompatibleClassChangeError" << std::endl;
            exit(1);
        }
        Class* _class = field->_class;
        if(OP == FIELD_OP::PUT && field->isFinal()){
            // final field can be initialized in <clinit>
            if(currentClass != _class || currentMethod->name != "<clinit>"){
                std::cerr << "java.lang.IllegalAccessError" << std::endl;
                exit(1);
            }
        }

        std::string& descriptor = field->descriptor;
        unsigned int slotId = field->slotId;
        Slots& slots = _class->staticVars;
        if(!IS_STATIC){
            // pop ref of its instance
            Ref ref = frame->pop<Ref>();
            if(ref == nullptr){
                std::cerr << "java.lang.NullPointerException" << std::endl;
                exit(1);
            }
            slots = ((Object*)ref)->fields;
        }   

        switch(descriptor[0]){
            case 'Z': case 'B': case 'C': case 'S': case 'I': operate<int>(frame, slots, slotId); break;
            case 'F': operate<float>(frame, slots, slotId); break;
            case 'J': operate<long>(frame, slots, slotId); break;
            case 'D': operate<double>(frame, slots, slotId); break;
            case 'L': case '[': operate<Ref>(frame, slots, slotId); break;
        }
    }
};

// Set static field in class
typedef FIELD<FIELD_OP::PUT, true>  PUT_STATIC;
// Get static field from class
typedef FIELD<FIELD_OP::GET, true>  GET_STATIC;
// Set field in object
typedef FIELD<FIELD_OP::PUT, false> PUT_FIELD;
// Fetch field from object
typedef FIELD<FIELD_OP::GET, false> GET_FIELD;

// Determine if object is of given type
struct INSTANCE_OF : public Index16Instruction{
    void execute(Frame* frame){
        Ref ref = frame->pop<Ref>();
        if(ref == nullptr){
            frame->push(0);
            return ;
        }
        ConstantPool* cp = frame->method->_class->constantPool;
        ClassRef* classRef = (ClassRef*)cp->getConstant(index).ref;
        Class* _class = classRef->resolvedClass();
        if(((Object*)ref)->isInstanceOf(_class)){
            frame->push(1);
        }
        else{
            frame->push(0);
        }
    }
};

// Check whether object is of given type
// difference between 'INSTANCE_OF': 
//  - make no change to operand_stack
//  - throw exception as a result
struct CHECK_CAST : public Index16Instruction{
    void execute(Frame* frame){
        Ref ref = frame->pop<Ref>();
        frame->push(ref);
        if(ref == nullptr) return ;
        ConstantPool* cp = frame->method->_class->constantPool;
        ClassRef* classRef = (ClassRef*)cp->getConstant(index).ref;
        Class* _class = classRef->resolvedClass();
        if(!((Object*)ref)->isInstanceOf(_class)){
            std::cerr << "java.lang.ClassCastException" << std::endl;
            exit(1);
        }
    }
};

#endif