#ifndef OBJECT_GUARD
#define OBJECT_GUARD
#include "../rtda/slot.h"
#include "class.h"

struct Object{
    Class* _class;
    Slots fields;

    Object(Class* _class): _class(_class), fields(Slots(_class->instanceSlotCount)){}

    bool isInstanceOf(Class* _class) const { return true; }
};

#endif