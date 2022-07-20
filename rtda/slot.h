#ifndef SLOT_GUARD
#define SLOT_GUARD
#include "../utils/utils.h"

union Slot{
    int num;
    Ref ref;
    Slot(int num):num(num){}
    Slot(Ref ref = nullptr):ref(ref){}
};


#endif