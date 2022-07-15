#include "../utils/utils.h"
#include "constant_pool.h"
using namespace std;

const ConstantInfo ConstantPool::getConstantInfo(uint16_t index) const { return constantInfos[index]; }

const ConstantPool readConstantPool(ClassReader& cr){
    uint16_t n = cr.readUint16();
    ConstantPool cp(n);
    vector<ConstantInfo> constantInfos(n);
    // 1 to n-1
    for(uint16_t i = 1; i < n; i++){
        constantInfos[i] = readConstantInfo(cr, cp);
    }
    return cp;
}

