#ifndef CLASS_LOADER_GUARD
#define CLASS_LOADER_GUARD
#include <map>
#include "class.h"
#include "../classpath/classpath.h"

class Class;
struct Field;

class ClassLoader{
private:
    Classpath* cp;
    std::map<std::string, Class*> classMap;
    bool verboseFlag;
public:
    ClassLoader(Classpath* cp, bool verboseFlag = false): cp(cp), verboseFlag(verboseFlag) {} 
    ~ClassLoader();
    
    Class* loadClass(const std::string& name);
    Class* loadNonArrayClass(const std::string& name);
    Class* loadArrayClass(const std::string& name);
    Class* defineClass(const std::vector<Byte>& data);
    Class* parseClass(const std::vector<Byte>& data);
    std::pair<std::vector<Byte>, Entry*> readClass(const std::string& name);
};

#endif