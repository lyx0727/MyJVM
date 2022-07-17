#include <iostream>
#include "local_vars.h"
#include "operand_stack.h"
#include <cstring>
using namespace std;

void testLocalVars() {
    LocalVars vars(1024);
    vars.set(0, 100);
    vars.set(1, -100);
    vars.set(2, 2997924580);
    vars.set(4, -2997924580);
    vars.set(6, 3.1415926);
    vars.set(7, 2.71828182845);
    vars.set(9, nullptr);
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
    ops.push(2997924580);
    ops.push(-2997924580);
    ops.push(3.1415926);
    ops.push(2.71828182845);
    ops.push(nullptr);
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