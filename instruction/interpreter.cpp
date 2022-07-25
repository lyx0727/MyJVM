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
    thread.pushFrame(frame);
    loop(thread, method->code);
}

void catchErr(Frame* frame){
    #ifdef DEBUG
        cout << "---------------local vars----------------" << endl;
        cout << frame->localVars.toString() << endl;
        cout << "-------------operand stack---------------" << endl;
        cout << frame->operandStack.toString() << endl;
        cout << "-----------------------------------------" << endl;
    #endif
    exit(1);
}

void loop(Thread& thread, const std::vector<Byte>& code){
    BytecodeReader reader;
    while(true){
        Frame* frame = thread.getCurrentFrame();
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
        cout << "pc: " << pc << " inst: " << getInstrucionName(opCode) << endl;
        inst->execute(frame);
        #ifdef DEBUG
            cout << "---------------local vars----------------" << endl;
            cout << frame->localVars.toString() << endl;
            cout << "-------------operand stack---------------" << endl;
            cout << frame->operandStack.toString() << endl;
            cout << "-----------------------------------------" << endl;
        #endif
        if(thread.empty()){
            break;
        }
    }
}