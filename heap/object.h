#ifndef OBJECT_GUARD
#define OBJECT_GUARD
#include "../rtda/slot.h"
#include "class.h"

struct Object{
    Class* _class;
    Slots fields;
};


#endif