#ifndef MEMBER_INFO_GUARD
#define MEMBER_INFO_GUARD
#include <vector>
#include "class_reader.h"

struct MemberInfo{};

const std::vector<MemberInfo*> readMembers(ClassReader& cr);

#endif