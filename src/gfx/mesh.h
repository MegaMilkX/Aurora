#ifndef MESH_H
#define MESH_H

#include <vector>
#include <iostream>
#include <string>

#include "gl/glbuffer.h"

namespace Au{
namespace GFX{
    
struct AttribInfo
{
    std::string name;
    unsigned int elemType;
    unsigned char elemCount;
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
    Attribute(std::string name) : name(name) {}
    operator AttribInfo() const
    {
        AttribInfo info;
        info.elemType = 0;
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
};

#define DEFINE_MESH_ATTRIB(NAME, TYPE, COUNT) \
class NAME : public Attribute<TYPE, COUNT> \
{ \
public: \
    NAME() \
    : Attribute<TYPE, COUNT>(#NAME){} \
}

DEFINE_MESH_ATTRIB(Position, float, 3);
DEFINE_MESH_ATTRIB(Normal, float, 3);
DEFINE_MESH_ATTRIB(ColorRGBA, char, 4);
DEFINE_MESH_ATTRIB(ColorRGB, char, 3);
DEFINE_MESH_ATTRIB(UV, float, 2);

class Mesh
{
public:
    void Format(const std::vector<AttribInfo>& vertexFormat)
    {
        this->vertexFormat = vertexFormat;
    }
    
    void PrintFormat()
    {
        for(unsigned int i = 0; i < vertexFormat.size(); ++i)
        {
            std::cout << "Attrib: " << std::endl;
            std::cout << vertexFormat[i].name << std::endl;
            std::cout << (int)(vertexFormat[i].elemCount) << std::endl;
        }
    }
private:
    std::vector<AttribInfo> vertexFormat;
    GLBuffer vertexBuffer;
    GLBuffer indexBuffer;
};

}
}

#endif
