#include <vector>
#include <cstdint>
#include "interpreter.h"
#include "../rtda/thread.h"
#include "bytecode_reader.h"
#include "instruction.h"
using namespace std;
using namespace classfile;

void interpret(MemberInfo* memberInfo, bool logInst){
    Method* method = new Method(nullptr, memberInfo);
    interpret(method, logInst);
    delete method;
}

void interpret(Method* method, bool logInst){
    Thread thread;
    Frame* frame = thread.newFrame(method);
    thread.pushFrame(frame);
    loop(thread, logInst);
}

void catchErr(Thread& thread){
    logFrames(thread);
    exit(1);
}

void logFrames(Thread& thread){
    while(!thread.empty()){
        Frame* frame = thread.popFrame();
        Method* method = frame->method;
        const string& className = method->_class->name;
        cout << ">> pc:" << frame->nextPc << " " << className << "." << method->name << method->descriptor << endl; 
    }
}

void logInstruction(Frame* frame, uint8_t opCode){
    Method* method = frame->method;
    const string& methodName = method->name;
    const string& className = method->_class->name;
    int pc = frame->thread->getPc();
    cout << className << "." << methodName << " #" << pc << " " << getInstrucionName(opCode) << endl;
}

void loop(Thread& thread, bool logInst){
    BytecodeReader reader;
    while(true){
        Frame* frame = thread.getCurrentFrame();
        int pc = frame->nextPc;
        thread.setPc(pc);
        // decode
        reader.reset(frame->method->code, pc);
        uint8_t opCode = reader.readUint8();
        Instruction* inst = getInstrucion(opCode);
        inst->fetchOperands(reader);
        frame->nextPc = reader.getPc();
        if(logInst){
            logInstruction(frame, opCode);
        }
        // execute
        try{
            inst->execute(frame);
        }
        catch(JavaLangException& e){
            cerr << e.what() << endl;
            catchErr(thread);
        }
        if(thread.empty()){
            break;
        }
    }
}