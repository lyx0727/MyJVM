#include <vector>
#include <cstdint>
#include "interpreter.h"
#include "../rtda/thread.h"
#include "bytecode_reader.h"
#include "instruction.h"
using namespace std;
using namespace classfile;

void interpret(MemberInfo* memberInfo){
    Method* method = new Method(nullptr, memberInfo);
    interpret(method);
    delete method;
}

void interpret(Method* method){
    Thread thread;
    Frame* frame = thread.newFrame(method);
    cout << method->name << ":" << to_string(method->maxLocals) << " " << to_string(method->maxStack) << endl;
    thread.pushFrame(frame);
    loop(thread, method->code);
}

void catchErr(Frame* frame){
    cout << frame->localVars.toString() << endl;
    exit(1);
}

void loop(Thread& thread, const std::vector<Byte>& code){
    Frame* frame = thread.popFrame();
    BytecodeReader reader;
    while(true){
        int pc = frame->nextPc;
        thread.setPc(pc);
        // decode
        reader.reset(code, pc);
        uint8_t opCode = reader.readUint8();
        Instruction* inst = getInstrucion(opCode);
        if(inst == nullptr){
            catchErr(frame);
        }
        inst->fetchOperands(reader);
        frame->nextPc = reader.getPc();
        // execute
        cout << "pc: " << pc << " inst: " << typeid(*inst).name() << endl;
        try{
            inst->execute(frame);
        }catch(const exception& e){ cout << e.what() << endl;}
    }
}