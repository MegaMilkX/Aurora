#ifndef MESH_H
#define MESH_H

#include "gl/glbuffer.h"

#include "attribute.h"

namespace Au{
namespace GFX{

class Mesh
{
public:
    Mesh();
    ~Mesh();
    
    enum PRIMITIVE
    {
        POINT,
        LINE,
        TRIANGLE
    };
    
    void PrimitiveType(PRIMITIVE prim);
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
    
    GLenum primitive;
};

template<typename VERTEX>
void Mesh::VertexData(const std::vector<VERTEX>& data)
{
    VertexData((void*)data.data(), data.size());
}

}
}

#endif
