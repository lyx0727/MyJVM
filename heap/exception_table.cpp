#include "exception_table.h"
using namespace std;

ExceptionTable::ExceptionTable(
    const vector<classfile::ExceptionTableEntry>& entrys, ConstantPool* cp)
    : handlers(entrys.size()){
    for(size_t i = 0; i < entrys.size(); i++){
        const classfile::ExceptionTableEntry& entry = entrys[i];
        handlers[i] = new ExceptionHandler(
            entry.startPc, 
            entry.endPc, 
            entry.handlerPc, 
            cp->getCatchType(entry.catchType)
        );
    }
}

ExceptionTable::~ExceptionTable(){
    for(ExceptionHandler* handler : handlers){
        if(handler){
            delete handler;
        }
    }
}

ExceptionHandler* ExceptionTable::findExceptionHandler(Class* exClass, int pc){
    for(ExceptionHandler* handler : handlers){
        if(pc >= handler->startPc && pc < handler->endPc){
            if(handler->catchType == nullptr){
                // catch all
                return handler;
            }
        }  
        Class* catchClass = handler->catchType->resolvedClass();    
        if(catchClass == exClass || catchClass->isSuperClassOf(exClass)){
            return handler;
        } 
    }
    return nullptr;
}