#include <iostream>
#include "local_vars.h"
#include "operand_stack.h"
#include <cstring>
using namespace std;

void testLocalVars() {
    LocalVars vars(1024);
    vars.set(0, 100);
    vars.set(1, -100);
    vars.set(2, 2997924580L);
    vars.set(4, -2997924580L);
    vars.set(6, 3.1415926F);
    vars.set(7, 2.71828182845);
    char* ref = new char;
    vars.set(9, ref);
    delete ref;
    cout << "---------local vars----------" << endl;
    cout << vars.get<int>(0) << endl;
    cout << vars.get<int>(1) << endl;
    cout << vars.get<long>(2) << endl;
    cout << vars.get<long>(4) << endl;
    cout << vars.get<float>(6) << endl;
    cout << vars.get<double>(7) << endl;
    cout << vars.get<Ref>(9) << endl;
}

void testOperandStack(){
    OperandStack ops(1024);
    ops.push(100);
    ops.push(-100);
    ops.push(2997924580L);
    ops.push(-2997924580L);
    ops.push(3.1415926F);
    ops.push(2.71828182845);
    char* ref = new char;
    ops.push(ref);
    delete ref;
    cout << "---------operand stack----------" << endl;
    cout << ops.pop<Ref>() << endl;
    cout << ops.pop<double>() << endl;
    cout << ops.pop<float>() << endl;
    cout << ops.pop<long>() << endl;
    cout << ops.pop<long>() << endl;
    cout << ops.pop<int>() << endl;
    cout << ops.pop<int>() << endl;
}

int main(){
    testLocalVars();
    testOperandStack();

    return 0;
}