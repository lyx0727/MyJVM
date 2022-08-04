#ifndef HEAP_EXCEPTION_TABLE_GUARD
#define HEAP_EXCEPTION_TABLE_GUARD
#include "class.h"
#include "../classfile/classfile.h"

struct Class;
struct ClassRef;
struct ConstantPool;

struct ExceptionHandler {
    int       startPc;
    int       endPc;
    int       handlerPc;
    ClassRef* catchType;
    ExceptionHandler(int startPc, int endPc, int handlerPc, ClassRef* catchType)
    : startPc(startPc), endPc(endPc), handlerPc(handlerPc), catchType(catchType) {}
};

struct ExceptionTable {
private:
    std::vector<ExceptionHandler*> handlers;
public:
    ExceptionTable(const std::vector<classfile::ExceptionTableEntry>& entrys, ConstantPool* cp);
    ~ExceptionTable();

    ExceptionHandler* findExceptionHandler(Class* exClass, int pc);
};

#endif