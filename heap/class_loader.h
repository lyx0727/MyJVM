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
public:
    ClassLoader(Classpath* cp): cp(cp){}
    Class* loadClass(const std::string& name);
    Class* loadNonArrayClass(const std::string& name);
    Class* defineClass(const std::vector<Byte>& data);
    Class* parseClass(const std::vector<Byte>& data);
    std::pair<std::vector<Byte>, Entry*> readClass(const std::string& name);
};

void resolveSuperClass(Class* _class);
void resolveInterfaces(Class* _class);

void verify(Class* _class);
void prepare(Class* _class);
void link(Class* _class);

void calcInstanceFieldSlotIds(Class* _class);
void calcStaticFieldSlotIds(Class* _class);
void allocAndInitStaticVars(Class* _class);

void initStaticFinalVar(Class* _class, Field* field);

#endif