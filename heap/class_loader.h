#ifndef CLASS_LOADER_GUARD
#define CLASS_LOADER_GUARD
#include <map>
#include "class.h"
#include "../classpath/classpath.h"

struct Class;
struct Field;
struct Object;

const uint8_t AT_BOOLEAN = 4;
const uint8_t AT_CHAR    = 5;
const uint8_t AT_FLOAT   = 6;
const uint8_t AT_DOUBLE  = 7;
const uint8_t AT_BYTE    = 8;
const uint8_t AT_SHORT   = 9;
const uint8_t AT_INT     = 10;
const uint8_t AT_LONG    = 11;

class ClassLoader{
private:
    Classpath* cp;
    std::map<std::string, Class*> classMap;
    std::map<std::string, Object*> internedStrings;
    bool verboseFlag;
public:
    ClassLoader(Classpath* cp, bool verboseFlag = false): cp(cp), verboseFlag(verboseFlag) {} 
    ~ClassLoader();
    
    Class* loadClass(const std::string& name);
    Class* loadNonArrayClass(const std::string& name);
    Class* loadArrayClass(const std::string& name);
    std::pair<std::vector<Byte>, Entry*> readClass(const std::string& name);
    Class* parseClass(const std::vector<Byte>& data);
    Class* defineClass(const std::vector<Byte>& data);
    Class* getPrimitiveArrayClass(uint8_t atype);

    Object* JString(const std::string& str);
    Object* createArgsArray(const std::vector<std::string>& args);
};

#endif