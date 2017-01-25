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
        case 'i':
            stride = 4;
            std::cout << "Some array, size: " << data.size() / stride;
            break;
        case 'd':
        case 'l':
            stride = 8;
            std::cout << "Some array, size: " << data.size() / stride;
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
        return std::string(data.data(), data.data() + data.size());
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
    
    void Name()
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
{};

class Reader
{
public:
    Reader()
    {}
    bool ReadFile(const char* data, unsigned size);
    
    template<typename T>
    std::vector<T> GetVertices(unsigned object = 0);
    template<typename T>
    std::vector<T> GetNormals(unsigned object = 0);
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
    
    Node rootNode;
};

template<typename T>
std::vector<T> Reader::GetVertices(unsigned object)
{
    return rootNode.Get("Geometry", object).Get("Vertices")[0].GetArray<T>();
}

template<typename T>
std::vector<T> Reader::GetNormals(unsigned object)
{
    std::vector<float> temp = 
        rootNode.Get("Geometry", object).Get("LayerElementNormal").Get("Normals")[0].GetArray<float>();
    std::string mapping = 
        rootNode.Get("Geometry", object).Get("LayerElementNormal").Get("MappingInformationType")[0].GetString();
    std::vector<float> vertices = GetVertices<float>(object);
    
    std::vector<T> result(vertices.size());
    if(mapping == "ByVertex" || mapping == "ByVertice")
    {
        for(unsigned i = 0; i < temp.size(); ++i)
        {
            result[i] = ((T)temp[i]);
        }
    }
    else if(mapping == "ByPolygon")
    {
        std::vector<int32_t> indices = rootNode.Get("Geometry", object).Get("PolygonVertexIndex")[0].GetArray<int32_t>();
        unsigned index = 0;
        for(unsigned i = 0; i < temp.size() / 3; ++i)
        {
            std::vector<unsigned> polyIndices;
            for(unsigned j = index; j < indices.size(); ++j)
            {
                index = j + 1;
                int32_t idx = indices[j];
                if(idx < 0)
                {
                    idx = -idx - 1;
                    polyIndices.push_back(idx);
                    break;
                }
                polyIndices.push_back(idx);
            }
            
            for(unsigned j = 0; j < polyIndices.size(); ++j)
            {
                result[polyIndices[j] * 3] = temp[i * 3];
                result[polyIndices[j] * 3 + 1] = temp[i * 3 + 1];
                result[polyIndices[j] * 3 + 2] = temp[i * 3 + 2];
            }
        }
    }
    else if(mapping == "ByPolygonVertex")
    {
        std::vector<int32_t> indices = rootNode.Get("Geometry", object).Get("PolygonVertexIndex")[0].GetArray<int32_t>();
        for(unsigned i = 0; i < indices.size(); ++i)
        {
            int32_t index = indices[i];
            if(index < 0)
                index = -index - 1;
            
            result[index * 3] = temp[i * 3];
            result[index * 3 + 1] = temp[i * 3 + 1];
            result[index * 3 + 2] = temp[i * 3 + 2];
        }
    }
    /*
    // TODO
    else if(mapping == "ByEdge")
    {
        
    }
    else if(mapping == "AllSame")
    {
        
    }
    */
    return result;
}

template<typename T>
std::vector<T> Reader::GetUV(unsigned object)
{
    // TODO
    return std::vector<T>();
}

template<typename T>
std::vector<T> Reader::GetIndices(unsigned object)
{
    std::vector<int32_t> temp = rootNode.Get("Geometry", object).Get("PolygonVertexIndex")[0].GetArray<int32_t>();
    std::vector<T> result;
    for(unsigned i = 0; i < temp.size(); ++i)
    {
        if(temp[i] < 0)
            temp[i] = -temp[i] - 1;
        result.push_back((T)temp[i]);
    }
    return result;
}

}
}
}

#endif
