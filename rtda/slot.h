#ifndef SLOT_GUARD
#define SLOT_GUARD
#include <vector>
#include <cstdint>
#include "../utils/utils.h"

union Slot{
    int num;
    Ref ref;
    Slot(int num):num(num){}
    Slot(Ref ref = nullptr):ref(ref){}
};

class Slots{
private:
    std::vector<Slot> slots;
public:
    Slots(unsigned int maxLocals = 0U): slots(std::vector<Slot>(maxLocals)){}

    size_t size() const { return slots.size(); }
    void set(size_t index, int val){ slots[index].num = val; }
    void set(size_t index, float val){ slots[index].num = to<int>(val); }
    void set(size_t index, long val){ slots[index].num = (int)val; slots[index + 1].num = (int)(val >> 32); }
    void set(size_t index, double val){ set(index, to<long>(val)); }
    void set(size_t index, Ref ref){ slots[index].ref = ref; }
    int getInt(size_t index){ return slots[index].num; }
    float getFloat(size_t index){ return to<float>(slots[index].num); }
    // operator preference: '|' > '<<' 
    long getLong(size_t index){ return (to<long>(slots[index + 1].num) << 32) | to<long>(slots[index].num); }
    double getDouble(size_t index){ return to<double>(getLong(index)); }
    Ref getRef(size_t index){ return slots[index].ref; }

    template<typename T> T get(size_t index);

    const std::string toString() const;
};

template<> inline int Slots::get(size_t index){ return getInt(index); }
template<> inline float Slots::get(size_t index){ return getFloat(index); }
template<> inline long Slots::get(size_t index){ return getLong(index); }
template<> inline double Slots::get(size_t index){ return getDouble(index); }
template<> inline Ref Slots::get(size_t index){ return getRef(index); }

#endif