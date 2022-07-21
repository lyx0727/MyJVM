#ifndef CONTROL_INSTRUCTION_GUARD
#define CONTROL_INSTRUCTION_GUARD
#include "instruction.h"

// Branch always
struct GOTO : public BranchInstruction{
    void execute(Frame* frame){
        frame->branch(offset);
    }
};

// Access jump table by index and jump
struct TABLE_SWITCH : public Instruction{
    int defaultOffset;
    int* jumpOffsets;
    int low;
    int high;
    ~TABLE_SWITCH(){ delete[] jumpOffsets; }
    void fetchOperands(BytecodeReader& br){
        br.skipPadding();
        defaultOffset = br.readInt32();
        low = br.readInt32();
        high = br.readInt32();
        jumpOffsets = br.readInt32s(high - low + 1);
    }
    void execute(Frame* frame){
        int index = frame->pop<int>();
        int offset;
        if(index >= low && index <= high){
            offset = jumpOffsets[index - low];
        }
        else{
            offset = defaultOffset;
        }
        frame->branch(offset);
    }
};

struct LOOKUP_SWITCH : public Instruction{
    int defaultOffset;
    int npairs;
    int* matchOffsets;
    ~LOOKUP_SWITCH(){ delete[] matchOffsets; }
    void fetchOperands(BytecodeReader& br){
        br.skipPadding();
        defaultOffset = br.readInt32();
        npairs = br.readInt32();
        matchOffsets = br.readInt32s(npairs * 2);
    }
    void execute(Frame* frame){
        int key = frame->pop<int>();
        for(int i = 0; i < npairs * 2; i += 2){
            if(matchOffsets[i] == key){
                frame->branch(matchOffsets[i + 1]);
                return;
            }
        }
        frame->branch(defaultOffset);
    }
};

#endif