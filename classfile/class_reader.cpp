#include "class_reader.h"
using namespace std;

const uint8_t ClassReader::readUint8(){
    uint8_t val = bytes[cur];
    cur++;
    return val;
}

const uint16_t ClassReader::readUint16(){
    uint16_t val = 
        ((uint16_t)bytes[cur] << 8) 
        + (uint16_t)bytes[cur + 1];
    cur += 2;
    return val;
}

const uint32_t ClassReader::readUint32(){
    uint32_t val = 
        ((uint32_t)bytes[cur] << 24) 
        + ((uint32_t)bytes[cur + 1] << 16) 
        + ((uint32_t)bytes[cur + 2] << 8) 
        + (uint32_t)bytes[cur + 3];
    cur += 4;
    return val;
}

const uint64_t ClassReader::readUint64(){
    uint64_t val = 
        ((uint64_t)bytes[cur] << 56) 
        + ((uint64_t)bytes[cur + 1] << 48) 
        + ((uint64_t)bytes[cur + 2] << 40) 
        + ((uint64_t)bytes[cur + 3] << 32) 
        + ((uint64_t)bytes[cur + 4] << 24) 
        + ((uint64_t)bytes[cur + 5] << 16) 
        + ((uint64_t)bytes[cur + 6] << 8) 
        + (uint64_t)bytes[cur + 7];
    cur += 8;
    return val;
}

const vector<uint16_t> ClassReader::readUint16s(){
    // amount to read depends on the first uint16 number 'n'
    uint16_t n = readUint16();
    vector<uint16_t> vals;
    for(uint16_t i = 0; i < n; i++){
        vals.push_back(readUint16());
    }
    return vals;
}

const vector<Byte> ClassReader::readBytes(uint32_t n){
    vector<Byte> vals;
    for(uint32_t i = 0; i < n; i++){
        vals.push_back(readUint8());
    }
    return vals;
}
