#ifndef LOCAL_VARS_GUARD
#define LOCAL_VARS_GUARD
#include <vector>
#include <cstdint>
#include "../utils/utils.h"
#include "slot.h"

class LocalVars{
private:
    std::vector<Slot> vars;
public:
    LocalVars(unsigned int maxLocals): vars(std::vector<Slot>(maxLocals)){}

    void set(size_t index, int val){ vars[index].num = val; }
    void set(size_t index, float val){ vars[index].num = toInt(val); }
    void set(size_t index, long val){ vars[index].num = (int)val; vars[index + 1].num = (int)(val >> 32); }
    void set(size_t index, double val){ set(index, toLong(val)); }
    void set(size_t index, Ref ref){ vars[index].ref = ref; }
    int getInt(size_t index){ return vars[index].num; }
    float getFloat(size_t index){ return toFloat(vars[index].num); }
    // operator preference: '|' > '<<' 
    long getLong(size_t index){ return (toLong(vars[index + 1].num) << 32) | toLong(vars[index].num); }
    double getDouble(size_t index){ return toDouble(getLong(index)); }
    Ref getRef(size_t index){ return vars[index].ref; }

    template<typename T> T get(size_t index);
};

template<> inline int LocalVars::get(size_t index){ return getInt(index); }
template<> inline float LocalVars::get(size_t index){ return getFloat(index); }
template<> inline long LocalVars::get(size_t index){ return getLong(index); }
template<> inline double LocalVars::get(size_t index){ return getDouble(index); }
template<> inline Ref LocalVars::get(size_t index){ return getRef(index); }

#endif