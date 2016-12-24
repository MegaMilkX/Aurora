#include "mesh.h"

namespace Au{
namespace GFX{
    
std::map<typeindex, GLenum> MakeGLTypeMap()
{
    std::map<typeindex, GLenum> typemap;
    typemap[TypeInfo<char>::Index()] = GL_BYTE;
    typemap[TypeInfo<unsigned char>::Index()] = GL_UNSIGNED_BYTE;
    typemap[TypeInfo<short>::Index()] = GL_SHORT;
    typemap[TypeInfo<unsigned short>::Index()] = GL_UNSIGNED_SHORT;
    typemap[TypeInfo<int>::Index()] = GL_INT;
    typemap[TypeInfo<unsigned int>::Index()] = GL_UNSIGNED_INT;
    // GL_HALF_FLOAT
    typemap[TypeInfo<float>::Index()] = GL_FLOAT;
    typemap[TypeInfo<double>::Index()] = GL_DOUBLE;
    // GL_FIXED
    // GL_INT_2_10_10_10_REV
    // GL_UNSIGNED_INT_2_10_10_10_REV
    // GL_UNSIGNED_INT_10F_11F_11F_REV
    
    return typemap;
}
    
GLenum GetGLType(typeindex index)
{
    static std::map<typeindex, GLenum> typemap = MakeGLTypeMap();
    return typemap[index];
}

Mesh::Mesh()
: vertexSize(0)
{
    glGenVertexArrays(1, &vao);
    Bind();
    vertexBuffer = GLBuffer::Create(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    vertexBuffer.Bind();
    indexBuffer = GLBuffer::Create(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
    indexBuffer.Bind();
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &vao);
    vertexBuffer.Destroy();
    indexBuffer.Destroy();
}

void Mesh::Format(const std::vector<AttribInfo>& vertexFormat)
{
    this->vertexFormat = vertexFormat;
    vertexSize = 0;
    for(unsigned int i = 0; i < vertexFormat.size(); ++i)
    {
        vertexSize += vertexFormat[i].elemSize * vertexFormat[i].elemCount;
    }
    
    Bind();
    
    size_t offset = 0;
    std::map<typeindex, AttribInstance> instanceCounters;
    for(unsigned int i = 0; i < vertexFormat.size(); ++i)
    {
        const AttribInfo& attrInfo = vertexFormat[i];
        AttribIndex globalAttribIndex = 
            GetGlobalAttribIndex(attrInfo.typeIndex,
                                 instanceCounters[attrInfo.typeIndex]++);
        GLboolean normalized = GL_FALSE;
        if(GetGLType(attrInfo.elemType) == GL_UNSIGNED_BYTE)
            normalized = GL_TRUE;
        glVertexAttribPointer(globalAttribIndex,                    //*attrib index
                              attrInfo.elemCount,                   //*attrib element count
                              GetGLType(attrInfo.elemType),         //*element type
                              normalized,                           //*normalize?
                              vertexSize,                           //*stride
                              (void*)offset);                       //*offset
        offset += attrInfo.elemSize * attrInfo.elemCount;
        glEnableVertexAttribArray(globalAttribIndex);
    }
}

void Mesh::VertexData(void* data, unsigned int count)
{
    if(vertexSize == 0)
        return;
    
    vertexBuffer.Data(data, vertexSize * count);
}

void Mesh::IndexData(const std::vector<unsigned short>& data)
{
    IndexData((void*)data.data(), data.size());
}

void Mesh::IndexData(void* data, unsigned int count)
{
    if(count == 0)
        return;
    
    indexBuffer.Data(data, sizeof(unsigned short) * count);
    indexCount = count;
    //index_type = GL_UNSIGNED_SHORT;
}

void Mesh::Bind()
{
    glBindVertexArray(vao);
}

void Mesh::Render()
{
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, 0);
}

void Mesh::PrintFormat()
{
    for(unsigned int i = 0; i < vertexFormat.size(); ++i)
    {
        vertexFormat[i].Print();
    }
}
 
}
}