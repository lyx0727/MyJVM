#ifndef INSTRUCTION_GUARD
#define INSTRUCTION_GUARD
#include <unordered_map>
#include "bytecode_reader.h"
#include "../rtda/frame.h"

struct Instruction{
    virtual void fetchOperands(BytecodeReader& br) = 0;
    virtual void execute(Frame* frame) = 0;
};

extern const std::unordered_map<uint8_t, Instruction*> Instructions;

struct NoOperandsInstruction : public Instruction{
    void fetchOperands(BytecodeReader& br){
        // do nothing
    }
    virtual void execute(Frame* frame) = 0;
};

struct BranchInstruction : public Instruction{
    int offset;
    void fetchOperands(BytecodeReader& br){
        offset = (int)br.readInt16();
    }
    virtual void execute(Frame* frame) = 0;
};

struct Index8Instruction : public Instruction{
    unsigned int index;
    void fetchOperands(BytecodeReader& br){
        index = (unsigned int)br.readUint8();
    }
    virtual void execute(Frame* frame) = 0;
};

struct Index16Instruction : public Instruction{
    unsigned int index;
    void fetchOperands(BytecodeReader& br){
        index = (unsigned int)br.readUint16();
    }
    virtual void execute(Frame* frame) = 0;
};

void branch(Frame* frame, int offset);

Instruction* getInstrucion(uint8_t opCode);
const std::string getInstrucionName(uint8_t opCode);

#endif