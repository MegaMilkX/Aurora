#ifndef MESH_H
#define MESH_H

#include <vector>
#include <iostream>
#include <string>

#include "gl/glbuffer.h"

#include "../util/typeinfo.h"

namespace Au{
namespace GFX{
    
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

#define DEFINE_MESH_ATTRIB(NAME, TYPE, COUNT) \
class NAME : public Attribute<TYPE, COUNT> \
{ \
public: \
    NAME() \
    : Attribute<TYPE, COUNT>(#NAME, TypeInfo<NAME>::Index()){} \
}

DEFINE_MESH_ATTRIB(Position, float, 3);
DEFINE_MESH_ATTRIB(Normal, float, 3);
DEFINE_MESH_ATTRIB(ColorRGBA, char, 4);
DEFINE_MESH_ATTRIB(ColorRGB, char, 3);
DEFINE_MESH_ATTRIB(UV, float, 2);

class Mesh
{
public:
    Mesh();
    ~Mesh();
    
    void Format(const std::vector<AttribInfo>& vertexFormat);
    template<typename VERTEX>
    void VertexData(const std::vector<VERTEX>& data);
    void VertexData(void* data, unsigned int count);
    void IndexData(const std::vector<unsigned short>& data);
    void IndexData(void* data, unsigned int count);
    
    void Bind();
    
    void Render();
    
    void PrintFormat();
private:
    std::vector<AttribInfo> vertexFormat;
    size_t vertexSize;
    unsigned int vao;
    GLBuffer vertexBuffer;
    GLBuffer indexBuffer;
    unsigned int indexCount;
};

template<typename VERTEX>
void Mesh::VertexData(const std::vector<VERTEX>& data)
{
    VertexData((void*)data.data(), data.size());
}

}
}

#endif
