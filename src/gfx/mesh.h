#ifndef MESH_H
#define MESH_H

namespace Au{
namespace GFX{
    
class AttributeBase
{
    
};

template<typename T, int count>
class Attribute : public AttributeBase
{
public:
    T& operator[](unsigned int index) { return value[index]; }
private:
    T value[count];
}

#define DEFINE_MESH_ATTRIB(NAME, TYPE, COUNT) \
template<int LAYER> \
class NAME : public Attribute<TYPE, COUNT> \
{}

DEFINE_MESH_ATTRIB(Position, float, 3);
DEFINE_MESH_ATTRIB(Normal, float, 3);
DEFINE_MESH_ATTRIB(ColorRGBA, char, 4);
DEFINE_MESH_ATTRIB(ColorRGB, char, 3);

class Mesh
{
public:
    
};

}
}

#endif
