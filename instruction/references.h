#ifndef INSTRUCTION_REFERENCES_GUARD
#define INSTRUCTION_REFERENCES_GUARD
#include <iostream>
#include "instruction.h"
#include "../heap/object.h"
#include "../heap/sym_ref.h"

struct NEW : public Index16Instruction{
    void execute(Frame* frame){
        ConstantPool* cp = frame->method->_class->constantPool;
        ClassRef* classRef =  (ClassRef*)cp->getConstant(index).getVal<Ref>();
        Class* _class = classRef->resolvedClass();
        if(!_class->initStarted){
            frame->revertNextPC();
            frame->initClass(_class);
            return;
        }
        if(_class->isAbstract() || _class->isInterface()){
            throw JavaLangInstantiationError(_class->name, __FILE__, __LINE__);
        }  
        Object* obj = new Object(_class);
        Ref ref = (Ref)obj;
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
    template<typename T> void operate(Frame* frame, Slots* slots, unsigned int slotId){
        T val;
        if(OP == FIELD_OP::PUT){
            val = frame->pop<T>();
        }
        if(!IS_STATIC){
            // pop ref of its instance
            Ref ref = frame->pop<Ref>();
            
            if(ref == nullptr){
                throw JavaLangNullPointerException(ref, __FILE__, __LINE__);
            }
            Object* obj = (Object*)ref;
            slots = obj->getFields();
        }
        if(OP == FIELD_OP::PUT){
            slots->set(slotId, val);
        }
        else if(OP == FIELD_OP::GET){
            frame->push(slots->get<T>(slotId));
        }
    }
public:
    void execute(Frame* frame){
        Method* currentMethod = frame->method;
        Class* currentClass = currentMethod->_class;
        ConstantPool* cp = currentClass->constantPool;
        FieldRef* fieldRef = (FieldRef*)cp->getConstant(index).getVal<Ref>();
        Field* field = fieldRef->resolvedField();
        Class* _class = field->_class;
        if(!_class->initStarted){
            frame->revertNextPC();
            frame->initClass(_class);
            return;
        }
        if(field->isStatic() != IS_STATIC){
            throw JavaLangIncompatibleClassChangeError(field->name, __FILE__, __LINE__);
        }
        if(OP == FIELD_OP::PUT && field->isFinal()){
            // final field can be initialized in <clinit>
            if(currentClass != _class || currentMethod->name != "<clinit>"){
                throw JavaLangIllegalAccessError(field->name, __FILE__, __LINE__);
            }
        }

        std::string& descriptor = field->descriptor;
        unsigned int slotId = field->slotId;
        Slots* slots = _class->staticVars;

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
        ClassRef* classRef = (ClassRef*)cp->getConstant(index).getVal<Ref>();
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
        ClassRef* classRef = (ClassRef*)cp->getConstant(index).getVal<Ref>();
        Class* _class = classRef->resolvedClass();
        Object* obj = (Object*)ref;
        if(!(obj->isInstanceOf(_class))){
            throw JavaLangClassCastException(obj->_class->name, __FILE__, __LINE__);
        }
    }
};

struct LDC : public Index8Instruction{ void execute(Frame* frame){ frame->ldc(index); } };
struct LDC_W : public Index16Instruction{ void execute(Frame* frame){ frame->ldc(index); } };
struct LDC2_W : public Index16Instruction{ void execute(Frame* frame){ frame->ldc_w(index); } };

// Invoke a class (static) method
struct INVOKE_STATIC : public Index16Instruction {
    void execute(Frame* frame){ 
        ConstantPool* cp = frame->getConstantPool();
        MethodRef* methodRef = (MethodRef*)cp->getConstant(index).getVal<Ref>();
        Class* resolvedClass = methodRef->resolvedClass();
        Method* resolvedMethod = methodRef->resolvedMethod();
        Class* _class = resolvedMethod->_class;
        if(!_class->initStarted){
            frame->revertNextPC();
            frame->initClass(_class);
            return;
        }
        // constructor must be declared by its class
        if(resolvedMethod->name == "<init>" && resolvedMethod->_class != resolvedClass){
            throw JavaLangNoSuchMethodError(resolvedMethod->name, __FILE__, __LINE__);
        }   
        if(!resolvedMethod->isStatic()){
            throw JavaLangIncompatibleClassChangeError(resolvedMethod->name + " is not static", __FILE__, __LINE__);
        }
        frame->invokeMethod(resolvedMethod);
    }
};

// Invoke instance method; 
// special handling for superclass, private, and instance initialization method invocations
struct INVOKE_SPECIAL : public Index16Instruction{
    void execute(Frame* frame){ 
        ConstantPool* cp = frame->getConstantPool();
        Class* currentClass = frame->method->_class;
        MethodRef* methodRef = (MethodRef*)cp->getConstant(index).getVal<Ref>();
        Class* resolvedClass = methodRef->resolvedClass();
        Method* resolvedMethod = methodRef->resolvedMethod();
        // constructor must be declared by its class
        if(resolvedMethod->name == "<init>" && resolvedMethod->_class != resolvedClass){
            throw JavaLangNoSuchMethodError(resolvedMethod->name, __FILE__, __LINE__);
        }   
        if(resolvedMethod->isStatic()){
            throw JavaLangIncompatibleClassChangeError(resolvedMethod->name + " is static", __FILE__, __LINE__);
        }
        // get 'this' pointer
        Ref ref = frame->operandStack.getRefFromTop(resolvedMethod->argSlotCount - 1); 
        if(ref == nullptr){
            throw JavaLangNullPointerException(ref, __FILE__, __LINE__);
        }
        // can be accessed or not
        Object* obj = (Object*)ref;
        if(resolvedMethod->isProtected() 
        && resolvedMethod->_class->isSuperClassOf(currentClass)
        && resolvedClass->getPackageName() != currentClass->getPackageName()
        && obj->_class != currentClass
        && !obj->_class->isSubClassOf(currentClass)){
            throw JavaLangIllegalAccessError(resolvedMethod->name, __FILE__, __LINE__);
        }
        // invoke method of super class
        Method* methodToBeInvoked = resolvedMethod;
        if(currentClass->isSuper() 
        && resolvedClass->isSuperClassOf(currentClass)
        && resolvedMethod->name != "<init>"){
            methodToBeInvoked = currentClass->superClass->lookupMethodInClass(methodRef->name, methodRef->descriptor);
        }
        if(methodToBeInvoked == nullptr || methodToBeInvoked->isAbstract()){
            throw JavaLangAbstractMethodError(methodToBeInvoked ? methodToBeInvoked->name : "", __FILE__, __LINE__);
        }
        frame->invokeMethod(methodToBeInvoked);
    }
};

// hack
void println(Frame* frame, const std::string& descriptor){
    const std::string& d = descriptor;
    std::string str;
    if(d == "(Z)V"){ str = std::to_string(frame->pop<int>() != 0); }
    else if(d == "(C)V"){ str = std::string(1, (char)frame->pop<int>()); }
    else if(d == "(B)V" || d == "(S)V" || d == "(I)V"){ str = std::to_string(frame->pop<int>()); }
    else if(d == "(J)V"){ str = std::to_string(frame->pop<long>()); }
    else if(d == "(F)V"){ str = std::to_string(frame->pop<float>()); }
    else if(d == "(D)V"){ str = std::to_string(frame->pop<double>()); }
    else if(d == "(Ljava/lang/String;)V"){
        Object* jStr = (Object*)frame->pop<Ref>();
        char16_t* chars = jStr->getRefVar("value", "[C")->getChars();
        str = utf16_to_utf8(std::u16string(chars));
    }
    else{
        std::cerr << "println: " << d << std::endl;
        exit(1);
    }
    std::cout << str << std::endl;
    frame->pop<Ref>();
}

// Invoke instance method; dispatch based on class
struct INVOKE_VIRTUAL : public Index16Instruction{
    void execute(Frame* frame){
        ConstantPool* cp = frame->getConstantPool();
        Class* currentClass = frame->method->_class;
        MethodRef* methodRef = (MethodRef*)cp->getConstant(index).getVal<Ref>();
        Class* resolvedClass = methodRef->resolvedClass();
        Method* resolvedMethod = methodRef->resolvedMethod();
        // constructor must be declared by its class
        if(resolvedMethod->name == "<init>" && resolvedMethod->_class != resolvedClass){
            throw JavaLangNoSuchMethodError(resolvedMethod->name, __FILE__, __LINE__);
        }   
        if(resolvedMethod->isStatic()){
            throw JavaLangIncompatibleClassChangeError(resolvedMethod->name + " is static", __FILE__, __LINE__);
        }
        // get 'this' pointer
        Ref ref = frame->operandStack.getRefFromTop(resolvedMethod->argSlotCount - 1); 
        if(ref == nullptr){
            // hack System.out.println()
            if(resolvedMethod->name == "println"){
                println(frame, resolvedMethod->descriptor);
                return;
            }
            throw JavaLangNullPointerException(ref, __FILE__, __LINE__);
        }
        // can be accessed or not
        Object* obj = (Object*)ref;
        if(resolvedMethod->isProtected() 
        && resolvedMethod->_class->isSuperClassOf(currentClass)
        && resolvedClass->getPackageName() != currentClass->getPackageName()
        && obj->_class != currentClass
        && !obj->_class->isSubClassOf(currentClass)){
            throw JavaLangIllegalAccessError(resolvedMethod->name, __FILE__, __LINE__);
        }
        // 
        Method* methodToBeInvoked = resolvedMethod;
        methodToBeInvoked = obj->_class->lookupMethodInClass(methodRef->name, methodRef->descriptor);
        if(methodToBeInvoked == nullptr || methodToBeInvoked->isAbstract()){
            throw JavaLangAbstractMethodError(methodToBeInvoked ? methodToBeInvoked->name : "", __FILE__, __LINE__);
        }
        
        frame->invokeMethod(methodToBeInvoked);
    }
};

// Invoke interface method
struct INVOKE_INTERFACE : public Instruction{
    unsigned int index;
    void fetchOperands(BytecodeReader& br){
        index = (unsigned int)br.readUint16();
        br.readUint8(); // count
        br.readUint8(); // zero
    }
    void execute(Frame* frame){
        ConstantPool* cp = frame->getConstantPool();
        MethodRef* methodRef = (MethodRef*)cp->getConstant(index).getVal<Ref>();
        Method* resolvedMethod = methodRef->resolvedMethod();
        if(resolvedMethod->isStatic() || resolvedMethod->isPrivate()){
            throw JavaLangIncompatibleClassChangeError(resolvedMethod->name, __FILE__, __LINE__);
        }
        // get 'this' pointer
        Ref ref = frame->operandStack.getRefFromTop(resolvedMethod->argSlotCount - 1); 
        if(ref == nullptr){
            throw JavaLangNullPointerException(ref, __FILE__, __LINE__);
        }
        Object* obj = (Object*)ref;
        if(!obj->_class->isImplements(methodRef->resolvedClass())){
            throw JavaLangIncompatibleClassChangeError("no implements for " + resolvedMethod->name, __FILE__, __LINE__);
        }
        Method* methodToBeInvoked = obj->_class->lookupMethodInClass(methodRef->name, methodRef->descriptor);
        if(methodToBeInvoked == nullptr || methodToBeInvoked->isAbstract()){
            throw JavaLangAbstractMethodError(methodToBeInvoked ? methodToBeInvoked->name : "", __FILE__, __LINE__);
        }
        if(!methodToBeInvoked->isPublic()){
            throw JavaLangIllegalAccessError(methodToBeInvoked->name + " is not public", __FILE__, __LINE__);
        }
        frame->invokeMethod(methodToBeInvoked);
    }
};

// Create new array of primitive
struct NEW_ARRAY : public Instruction { 
    uint8_t atype; 
    void fetchOperands(BytecodeReader& br){
        atype = br.readUint8();
    }
    void execute(Frame* frame){
        int count = frame->pop<int>();
        if(count < 0){
            throw JavaLangNegativeArraySizeException(count, __FILE__, __LINE__);
        }
        ClassLoader* classLoader = frame->getClassLoader();
        Class* arrClass = classLoader->getPrimitiveArrayClass(atype);
        Ref arr = (Ref)arrClass->newArray(count);
        frame->push(arr);
    }
};

// Create new array of reference
struct ANEW_ARRAY : public Index16Instruction{
    void execute(Frame* frame){
        ConstantPool* cp = frame->getConstantPool();
        ClassRef* classRef = (ClassRef*)cp->getConstant(index).getVal<Ref>();
        Class* componentClass = classRef->resolvedClass();
        int count = frame->pop<int>();
        if(count < 0){
            throw JavaLangNegativeArraySizeException(count, __FILE__, __LINE__);
        }
        Class* arrClass = componentClass->getArrayClass();
        Ref arr = (Ref)arrClass->newArray(count);
        frame->push(arr);
    }
};

// Get length of array
struct ARRAY_LENGTH : public NoOperandsInstruction{
    void execute(Frame* frame){
        Ref ref = frame->pop<Ref>(); 
        if(ref == nullptr){
            throw JavaLangNullPointerException(ref, __FILE__, __LINE__);
        }
        Object* arrRef = (Object*)ref;
        int arrLen = arrRef->length;
        frame->push(arrLen);
    }
};

// Create new multidimensional array
struct MULTI_ANEW_ARRAY : public Instruction {
    uint16_t index;
    uint8_t  dimensions;
    void fetchOperands(BytecodeReader& br){
        index = br.readUint16();
        dimensions = br.readUint8();
    }
    void execute(Frame* frame){
        ConstantPool* cp = frame->getConstantPool();
        ClassRef* classRef = (ClassRef*)cp->getConstant(index).getVal<Ref>();
        Class* arrClass = classRef->resolvedClass();
        int* counts = new int[dimensions];
        for(int i = dimensions - 1; i >= 0; i--){
            counts[i] = frame->pop<int>();
            if(counts[i] < 0){
                throw JavaLangNegativeArraySizeException(counts[i], __FILE__, __LINE__);
            }
        }
        Ref arr = (Ref)arrClass->newMultiDimensionalArray(counts, dimensions);
        delete[] counts;
        frame->push(arr);
    }
};

#endif