#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>
#include <iostream>
#include <vector>

#include "typeinfo.h"

#define AU_DEF_ATTRIB(NAME, ELEMTYPE, ELEMCOUNT) \
    class NAME{ \
    public: \
        operator unsigned int() const { return GetAttribIndex<NAME>(); } \
        operator AttribInfo() const \
        { \
            AttribInfo info; \
            info.typeIndex = TypeInfo<NAME>::Index(); \
            info.elemType = TypeInfo<ELEMTYPE>::Index(); \
            info.elemSize = sizeof(ELEMTYPE); \
            info.elemCount = ELEMCOUNT; \
            info.name = #NAME; \
            return info; \
        } \
        std::vector<AttribInfo> operator<<(const AttribInfo& other) \
        { \
            std::vector<AttribInfo> result; \
            result.push_back(*this); \
            result.push_back(other); \
            return result; \
        } \
        operator std::vector<AttribInfo>() const \
        { \
            std::vector<AttribInfo> result; \
            result.push_back(*this); \
            return result; \
        } \
    }

namespace Au{

inline unsigned int NextAttribIndex()
{
    static unsigned int index = 0;
    return index++;
}

template<typename A>
int GetAttribIndex()
{
    static unsigned int index = NextAttribIndex();
    return index;
}

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
    
    bool operator==(const AttribInfo& other) const
    {
        return typeIndex == other.typeIndex;
    }
    
    void Print() const
    {
        std::cout << name << ": " << std::endl;
        std::cout << "typeIndex: " << typeIndex << std::endl;
        std::cout << "elemType: " << elemType << std::endl;
        std::cout << "elemSize: " << (int)elemSize << std::endl;
        std::cout << "elemCount: " << (int)elemCount << std::endl;
    }
};

inline std::vector<AttribInfo> operator<<(const std::vector<AttribInfo>& left, const AttribInfo& right)
{
    std::vector<AttribInfo> result = left;
    result.push_back(right);
    return result;
}

AttribIndex GetGlobalAttribIndex(typeindex type, unsigned int instance);
    
AU_DEF_ATTRIB(Position, float, 3);

AU_DEF_ATTRIB(Normal, float, 3);
AU_DEF_ATTRIB(Tangent, float, 3);
AU_DEF_ATTRIB(Bitangent, float, 3);

AU_DEF_ATTRIB(UV, float, 2);
AU_DEF_ATTRIB(UVW, float, 3);

AU_DEF_ATTRIB(ColorRGBA, unsigned char, 4);
AU_DEF_ATTRIB(ColorRGB, unsigned char, 3);

AU_DEF_ATTRIB(BoneWeight4, unsigned char, 4);
AU_DEF_ATTRIB(BoneIndex4, int, 4);

}

#endif
