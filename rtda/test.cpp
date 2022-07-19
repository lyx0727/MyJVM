#include <iostream>
#include <cstring>
#include "local_vars.h"
#include "operand_stack.h"
using namespace std;

void testLocalVars() {
    LocalVars vars(1024);
    vars.set(0, 100);
    vars.set(1, -100);
    vars.set(2, 2997924580L);
    vars.set(4, -2997924580L);
    vars.set(6, 3.1415926F);
    vars.set(7, 2.71828182845);
    Object* obj = new Object;
    vars.set(9, obj);
    delete obj;
    cout << "---------local vars----------" << endl;
    cout << vars.getInt(0) << endl;
    cout << vars.getInt(1) << endl;
    cout << vars.getLong(2) << endl;
    cout << vars.getLong(4) << endl;
    cout << vars.getFloat(6) << endl;
    cout << vars.getDouble(7) << endl;
    cout << vars.getRef(9) << endl;
}

void testOperandStack(){
    OperandStack ops(1024);
    ops.push(100);
    ops.push(-100);
    ops.push(2997924580L);
    ops.push(-2997924580L);
    ops.push(3.1415926F);
    ops.push(2.71828182845);
    Object* obj = new Object;
    ops.push(obj);
    delete obj;
    cout << "---------operand stack----------" << endl;
    cout << ops.popRef() << endl;
    cout << ops.popDouble() << endl;
    cout << ops.popFloat() << endl;
    cout << ops.popLong() << endl;
    cout << ops.popLong() << endl;
    cout << ops.popInt() << endl;
    cout << ops.popInt() << endl;
}

int main(){
    testLocalVars();
    testOperandStack();

    return 0;
}