#include "../utils/utils.h"
#include "constant_pool.h"
using namespace std;

namespace classfile{
void readConstantPool(ClassReader& cr, ConstantPool& cp){
    uint16_t n = cr.readUint16();
    cp.resize(n);
    // 1 to n - 1
    for(uint16_t i = 1; i < n; i++){
        cp[i] = readConstantInfo(cr, cp);
        switch(cp[i]->getTag()){
            case ConstantInfoType::Long:
            case ConstantInfoType::Double:
            // take 2 places
                i++;
        }
    }
}

const pair<string, string> ConstantPool::getNameAndType(uint16_t index) const { 
    ConstantNameAndTypeInfo* c = (ConstantNameAndTypeInfo*)getConstantInfo(index);
    string name = getUtf8(c->nameIndex);
    string type = getUtf8(c->descriptorIndex);
    return std::make_pair(name, type);
}

const string ConstantPool::getClassName(uint16_t index) const { 
    ConstantClassInfo* c = (ConstantClassInfo*)getConstantInfo(index);
    return getUtf8(c->nameIndex);
} 

const string ConstantPool::getUtf8(uint16_t index) const { 
    ConstantUtf8Info* c = (ConstantUtf8Info*)getConstantInfo(index);
    return c->str;
}

const string ConstantPool::toString() const{
    string str = "[constant pool]\t" + to_string(constantInfos.size()) + "\n";
    size_t i = 0;
    for(ConstantInfo* c : constantInfos){
        if(c){
            str += "\t" + c->toString() + "\n";
            i++;
        }
        // // print 10 at most
        // if(i > 10){
        //     str += "\t...\n";
        //     break;
        // }
    }   
    return str;
}

ConstantPool::~ConstantPool(){ 
    for(ConstantInfo* c : constantInfos){
        if(c) delete c;
    }
}
}