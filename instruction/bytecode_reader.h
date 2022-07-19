#ifndef BYTECODE_READER_GUARD
#define BYTECODE_READER_GUARD
#include <vector>
#include <cstdint>
#include "../utils/utils.h"

class BytecodeReader{
private:
    std::vector<Byte> code;
    int pc;
public:
    BytecodeReader(){}
    void reset(const std::vector<Byte>& code, int pc){ this->code = code; this->pc = pc; };

    int getPc() const { return pc; }
    
    uint8_t readUint8(){ return (uint8_t)code[pc++]; }
    int8_t readInt8(){ return (int8_t)readUint8(); }
    uint16_t readUint16(){ 
        uint8_t b1 = readUint8();  
        uint8_t b2 = readUint8();
        return ((uint16_t)b1 << 8) | (uint16_t)b2;  
    }
    int16_t readInt16(){ return (int16_t)readUint16(); }
    int32_t readInt32(){
        uint8_t b1 = readUint8();  
        uint8_t b2 = readUint8();
        uint8_t b3 = readUint8();
        uint8_t b4 = readUint8();
        return ((int32_t)b1 << 24) | ((int32_t)b2 << 16) | ((int32_t)b3 << 8) | (int32_t)b4;  
    }
    void skipPadding(){
        while(pc % 4 != 0){
            readUint8();
        }
    }
    int32_t* readInt32s(int32_t n){
        int32_t* ints = new int32_t[n];
        for(int32_t i = 0; i < n; i++){
            ints[i] = readInt32();
        } 
    }
};

#endif