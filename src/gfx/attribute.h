#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <vector>
#include <iostream>
#include <string>
#include <map>

#include "../util/typeinfo.h"

namespace Au{
namespace GFX{
    
// Global attribute index
typedef unsigned int AttribIndex;
// A number that tells you which one of the same attribute type
// of the same vertex format this is
typedef unsigned int AttribInstance;

struct AttribInfo
{
    typeindex typeIndex;
    std::string name;
    typeindex elemType;
    unsigned char elemSize;
    unsigned char elemCount;
    
    void Print()
    {
        std::cout << name << ": " << std::endl;
        std::cout << "typeIndex: " << typeIndex << std::endl;
        std::cout << "elemType: " << elemType << std::endl;
        std::cout << "elemSize: " << (int)elemSize << std::endl;
        std::cout << "elemCount: " << (int)elemCount << std::endl;
    }
};
    
class AttributeBase
{
    
};

inline std::vector<AttribInfo> operator<<(const std::vector<AttribInfo>& left, const AttribInfo& right)
{
    std::vector<AttribInfo> result = left;
    result.push_back(right);
    return result;
}

template<typename T, int count>
class Attribute : public AttributeBase
{
public:
    Attribute() {}
    Attribute(std::string name, typeindex typeIndex)
    : name(name), myTypeIndex(typeIndex) {}
    operator AttribInfo() const
    {
        AttribInfo info;
        info.typeIndex = myTypeIndex;
        info.elemType = TypeInfo<T>::Index();
        info.elemSize = sizeof(T);
        info.elemCount = count;
        info.name = name;
        return info;
    }
    T& operator[](unsigned int index) { return value[index]; }
    std::vector<AttribInfo> operator<<(const AttribInfo& other)
    {
        std::vector<AttribInfo> result;
        result.push_back(*this);
        result.push_back(other);
        return result;
    }
protected:
    T value[count];
    std::string name;
    typeindex myTypeIndex;
};

AttribIndex GetGlobalAttribIndex(typeindex type, unsigned int instance);

#define DEFINE_MESH_ATTRIB(NAME, TYPE, COUNT) \
class NAME : public Attribute<TYPE, COUNT> \
{ \
public: \
    NAME() \
    : Attribute<TYPE, COUNT>(#NAME, TypeInfo<NAME>::Index()){} \
}

DEFINE_MESH_ATTRIB(Position, float, 3);
DEFINE_MESH_ATTRIB(Normal, float, 3);
DEFINE_MESH_ATTRIB(ColorRGBA, unsigned char, 4);
DEFINE_MESH_ATTRIB(ColorRGB, unsigned char, 3);
DEFINE_MESH_ATTRIB(UV, float, 2); 
 
}
}

#endif
