#ifndef FBX_H
#define FBX_H

#include <iostream>
#include <string>
#include <vector>

#include "../math/math.h"

namespace Au{
namespace Media{
namespace FBX{

typedef uint8_t Byte;
typedef uint32_t Word;
typedef uint64_t DoubleWord;

class Prop
{
public:
    void Print(unsigned level = 0)
    {
        for(unsigned i = 0; i < level; ++i)
            std::cout << "  ";
        int stride = 0;
        switch(type)
        {
        // 16 bit int
        case 'Y':
            std::cout << *(int16_t*)(data.data());
            break;
        // 1 bit bool
        case 'C':
            std::cout << "1 bit bool";
            break;
        // 32 bit int
        case 'I':
            std::cout << *(int32_t*)(data.data());
            break;
        case 'F':
            std::cout << *(float*)(data.data());
            break;
        // double
        case 'D':
            std::cout << *(double*)(data.data());
            break;
        // 64 bit int
        case 'L':
            std::cout << *(int64_t*)(data.data());
            break;
        // Binary data
        case 'R':
            std::cout << "Just binary data";
            break;
        case 'b':
            break;
        case 'f':
            stride = 4;
            std::cout << "Float array, size: " << data.size() / stride;
            break;
        case 'i':
            stride = 4;
            std::cout << "Int32 array, size: " << data.size() / stride;
            break;
        case 'd':
            stride = 8;
            std::cout << "Double array, size: " << data.size() / stride;
            break;
        case 'l':
            stride = 8;
            std::cout << "Int64 array, size: " << data.size() / stride;
            break;
        case 'S':
            std::cout << std::string(data.data(), data.data() + data.size());
            break;
        }
        
        std::cout << std::endl;
    }
    void Type(char type)
    {
        this->type = type;
    }
    void ArraySize(unsigned arraySize)
    {
        this->arraySize = arraySize;
    }
    void Data(const std::vector<char>& data)
    {
        this->data = data;
    }
    
    std::string GetString()
    {
        unsigned strLen = 0;
        for(unsigned i = 0; i < data.size(); ++i)
        {
            if(data[i] == '\0')
                break;
            strLen++;
        }
        return std::string(data.data(), data.data() + strLen);
    }
    
    int64_t GetInt64()
    {
        if(data.size() != sizeof(int64_t))
            return 0;
        return *(int64_t*)(data.data());
    }
    
    int32_t GetInt32()
    {
        if(data.size() != sizeof(int32_t))
            return 0;
        return *(int32_t*)(data.data());
    }
    
    template<typename T>
    std::vector<T> GetArray()
    {
        std::vector<T> result;
        
        unsigned byteLength = data.size();
        unsigned arrayLength = 0;
        unsigned targetElementSize = sizeof(T);
        
        switch(type)
        {
        case 'f': case 'i': arrayLength = byteLength / 4; break;
        case 'd': case 'l': arrayLength = byteLength / 8; break;
        }
        
        if(arrayLength == 0)
            return result;
        
        switch(type)
        {
        case 'f':
            {
                float* dataptr = (float*)(data.data());
                for(unsigned i = 0; i < arrayLength; ++i)
                    result.push_back((T)(dataptr[i]));
            }
            break;
        case 'i':
            {
                int32_t* dataptr = (int32_t*)(data.data());
                for(unsigned i = 0; i < arrayLength; ++i)
                    result.push_back((T)(dataptr[i]));
            }
            break;
        case 'd':
            {
                double* dataptr = (double*)(data.data());
                for(unsigned i = 0; i < arrayLength; ++i)
                    result.push_back((T)(dataptr[i]));
            }
            break;
        case 'l':
            {
                int64_t* dataptr = (int64_t*)(data.data());
                for(unsigned i = 0; i < arrayLength; ++i)
                    result.push_back((T)(dataptr[i]));
            }
            break;
        }
        
        return result;
    }
private:
    char type;
    unsigned arraySize;
    std::vector<char> data;
};

class Node
{
public:    
    void Add(const Node& node)
    {
        children.push_back(node);
    }
    
    void Add(const Prop& prop)
    {
        props.push_back(prop);
    }
    
    void Name(const std::string& name)
    {
        this->name = name;
    }
    
    std::string Name()
    {
        return name;
    }
    
    void PropCount(unsigned count)
    {
        propCount = count;
    }
    
    unsigned ChildCount()
    {
        return children.size();
    }
    
    Node& Get(unsigned index)
    {
        if(index >= children.size())
            children.resize(index + 1);
        
        return children[index];
    }
    
    Node& Get(const std::string& name, unsigned instance = 0)
    {
        unsigned instanceCounter = 0;
        for(unsigned i = 0; i < children.size(); ++i)
        {
            if(children[i].name == name && instance == instanceCounter++)
            {
                return children[i];
            }
        }
        
        Node node;
        node.Name(name);
        children.push_back(node);
        return children[children.size()-1];
    }
    
    Node& GetWhere(unsigned propId, const std::string& value)
    {
        for(unsigned i = 0; i < children.size(); ++i)
        {
            if(children[i][propId].GetString() == value)
            {
                return children[i];
            }
        }
        return children[0];
    }
    
    std::vector<Node> GetAll(const std::string& name)
    {
        std::vector<Node> result;
        for(unsigned i = 0; i < children.size(); ++i)
        {
            if(children[i].name == name)
                result.push_back(children[i]);
        }
        return result;
    }
    
    int Count(const std::string& name)
    {
        int result = 0;
        for(unsigned i = 0; i < children.size(); ++i)
        {
            if(children[i].name == name)
                result++;
        }
        return result;
    }
    
    Prop& operator[](unsigned index)
    {
        if(index >= props.size())
            props.resize(index+1);
        
        return props[index];
    }
    
    void Print(unsigned level = 0)
    {
        for(unsigned i = 0; i < level; ++i)
            std::cout << "  ";
        std::cout << name << " | " << "Prop count: " << propCount << std::endl;
        ++level;
        for(unsigned i = 0; i < props.size(); ++i)
            props[i].Print(level);
        for(unsigned i = 0; i < children.size(); ++i)
            children[i].Print(level);
    }
    
private:
    std::string name;
    unsigned propCount;
    std::vector<Prop> props;
    std::vector<Node> children;
};

class Bone
{
public:
    int64_t uid;
    int64_t parentUID;
    std::string name;
    Math::Mat4f bindTransform;
};

enum CoordSystem
{
    ORIGINAL,
    OPENGL,
    DIRECT3D
};

enum Axis
{
    AXIS_X = 0, AXIS_Y = 1, AXIS_Z = 2, AXIS_UNKNOWN
};

class Mesh
{
public:
    void OptimizeDuplicates()
    {
        for(unsigned i = 0; i < indices.size(); ++i)
            for(unsigned j = i + 1; j < indices.size(); ++j)
                if(IsSameVertex(indices[i], indices[j]))
                    EraseVertex(indices[j], indices[i]);
    }
    
    bool IsSameVertex(unsigned a, unsigned b)
    {
        if(a == b)
            return false;
        
        if(vertices[a * 3] != vertices[b * 3] || 
           vertices[a * 3 + 1] != vertices[b * 3 + 1] ||
           vertices[a * 3 + 2] != vertices[b * 3 + 2])
            return false;
        else
        {
            for(unsigned i = 0; i < normalLayers.size(); ++i)
            {
                if(normalLayers[i][a * 3] != normalLayers[i][b * 3] || 
                   normalLayers[i][a * 3 + 1] != normalLayers[i][b * 3 + 1] ||
                   normalLayers[i][a * 3 + 2] != normalLayers[i][b * 3 + 2])
                    return false;
            }
            
            for(unsigned i = 0; i < uvLayers.size(); ++i)
            {
                if(uvLayers[i][a * 2] != uvLayers[i][b * 2] || 
                   uvLayers[i][a * 2 + 1] != uvLayers[i][b * 2 + 1])
                    return false;
            }
            
            for(unsigned i = 0; i < rgbaLayers.size(); ++i)
            {
                if(rgbaLayers[i][a * 4] != rgbaLayers[i][b * 4] || 
                   rgbaLayers[i][a * 4 + 1] != rgbaLayers[i][b * 4 + 1] ||
                   rgbaLayers[i][a * 4 + 2] != rgbaLayers[i][b * 4 + 2] ||
                   rgbaLayers[i][a * 4 + 3] != rgbaLayers[i][b * 4 + 3])
                    return false;
            }
        }
        
        return true;
    }
    
    void EraseVertex(int id, int newId)
    {
        for(unsigned i = 0; i < indices.size(); ++i)
            if(indices[i] == id)
                indices[i] = newId;
            
        for(unsigned i = 0; i < indices.size(); ++i)
            if(indices[i] >= id)
                indices[i]--;
        vertices.erase(vertices.begin() + id * 3, vertices.begin() + id * 3 + 3);
        for(unsigned i = 0; i < normalLayers.size(); ++i)
        {
            normalLayers[i].erase(normalLayers[i].begin() + id * 3, normalLayers[i].begin() + id * 3 + 3);
        }
        for(unsigned i = 0; i < uvLayers.size(); ++i)
        {
            uvLayers[i].erase(uvLayers[i].begin() + id * 2, uvLayers[i].begin() + id * 2 + 2);
        }
        for(unsigned i = 0; i < rgbaLayers.size(); ++i)
        {
            rgbaLayers[i].erase(rgbaLayers[i].begin() + id * 4, rgbaLayers[i].begin() + id * 4 + 4);
        }
    }

    std::vector<int> indices;
    std::vector<float> vertices;
    std::vector<std::vector<float>> normalLayers;
    std::vector<std::vector<float>> uvLayers;
    std::vector<std::vector<unsigned char>> rgbaLayers;
};

class Reader
{
public:
    Reader()
    {}
    bool ReadFile(const char* data, unsigned size);
    
    void ConvertCoordSys(CoordSystem sys)
    { 
        coordSys = sys;     
    }
    
    Axis GetUpAxis()
    {
        int axis = (int)rootNode.Get("Properties70", 0).GetWhere(0, "UpAxis")[4].GetInt32();
        return FBXAxisToAxis(axis);
    }
    Axis GetFrontAxis()
    {
        int axis = (int)rootNode.Get("Properties70", 0).GetWhere(0, "FrontAxis")[4].GetInt32();
        return FBXAxisToAxis(axis);
    }
    Axis GetRightAxis()
    {
        int axis = (int)rootNode.Get("Properties70", 0).GetWhere(0, "CoordAxis")[4].GetInt32();
        return FBXAxisToAxis(axis);
    }
    int GetUpAxisSign()
    {
        return (int)rootNode.Get("Properties70", 0).GetWhere(0, "UpAxisSign")[4].GetInt32();
    }
    int GetFrontAxisSign()
    {
        return (int)rootNode.Get("Properties70", 0).GetWhere(0, "FrontAxisSign")[4].GetInt32();
    }
    int GetRightAxisSign()
    {
        return (int)rootNode.Get("Properties70", 0).GetWhere(0, "CoordAxisSign")[4].GetInt32();
    }
    
    int MeshCount() { return meshes.size(); }
    
    std::vector<float> Reader::GetVertices(unsigned object)
    {
        std::vector<float> vertices = meshes[object].vertices;
        ConvertVertexArray(vertices);
        return vertices;
    }

    std::vector<float> Reader::GetNormals(unsigned object = 0, unsigned layer = 0)
    {
        std::vector<float> normals = meshes[object].normalLayers[layer];
        ConvertVertexArray(normals);
        return normals;
    }
    template<typename T>
    std::vector<T> GetUV(unsigned object = 0);
    template<typename T>
    std::vector<T> GetIndices(unsigned object = 0);
    
    std::vector<Bone> GetBones();
    
    void Print()
    {
        rootNode.Print();
    }
private:
    void ReadData(Prop& prop, std::vector<char>& out, const char* data, const char*& cursor, const char* end);
    bool ReadBlock(Node& node, const char* data, const char*& cursor, const char* end, Word flags);
    bool ReadFileFBX(const char* data, unsigned size);
    
    Axis FBXAxisToAxis(unsigned axis)
    { 
        if(axis == 0)
            return AXIS_X;
        else if(axis == 1)
            return AXIS_Z;
        else if(axis == 2)
            return AXIS_Y;
        else
            return AXIS_UNKNOWN;
    }
    
    template<typename T>
    void ConvertVertexArray(std::vector<T>& vertices)
    {
        Axis up = GetUpAxis();
        Axis front = GetFrontAxis();
        Axis right = GetRightAxis();
    
        if(coordSys == OPENGL)
        {
            for(unsigned i = 0; i < vertices.size(); i += 3)
            {
                T x = vertices[i + right];
                T y = vertices[i + up];
                T z = vertices[i + front];
                
                vertices[i + AXIS_Y] = y;
                vertices[i + AXIS_Z] = -z;
            }
        }
        else if(coordSys == DIRECT3D)
        {
            // UNTESTED
            for(unsigned i = 0; i < vertices.size(); i += 3)
            {
                T x = vertices[i + right];
                T y = vertices[i + up];
                T z = vertices[i + front];
                
                vertices[i + AXIS_Y] = y;
                vertices[i + AXIS_Z] = z;
            }
        }
    }
    
    Node rootNode;
    CoordSystem coordSys;
    std::vector<Mesh> meshes;
};

template<typename T>
std::vector<T> Reader::GetUV(unsigned object)
{
    // TODO
    return std::vector<T>();
}

template<typename T>
std::vector<T> Reader::GetIndices(unsigned object)
{/*
    std::vector<int32_t> temp = rootNode.Get("Geometry", object).Get("PolygonVertexIndex")[0].GetArray<int32_t>();
    std::vector<T> result;
    for(unsigned i = 0; i < temp.size(); ++i)
    {
        if(temp[i] < 0)
            temp[i] = -temp[i] - 1;
        result.push_back((T)temp[i]);
    }*/
    std::vector<T> result;
    
    for(unsigned i = 0; i < meshes[object].indices.size(); ++i)
    {
        result.push_back((T)meshes[object].indices[i]);
    }
    
    return result;
}

}
}
}

#endif
