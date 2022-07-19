#ifndef SLOT_GUARD
#define SLOT_GUARD
#include "object.h"

union Slot{
    int num;
    Ref ref;
    Slot(int num):num(num){}
    Slot(Object* ref = nullptr):ref(ref){}
};


#endif