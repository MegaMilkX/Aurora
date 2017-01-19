#ifndef FBX_H
#define FBX_H

#include <iostream>
#include <string>
#include <vector>

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
        case 'd':
        case 'l':
            std::cout << "Some array";
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
    void Data(const std::vector<char>& data)
    {
        this->data = data;
    }
private:
    char type;
    std::vector<char> data;
};

class Node
{
public:    
    Node* Add(const Node& node)
    {
        children.push_back(node);
        return &(children[children.size()-1]);
    }
    
    void Name(const std::string& name)
    {
        this->name = name;
    }
    
private:
    std::string name;
    std::vector<Node> children;
};

class Reader
{
public:
    Reader()
    : currentNode(&rootNode) {}
    bool ReadFile(const char* data, unsigned size);
private:
    void ReadData(std::vector<char>& out, const char* data, const char*& cursor, const char* end);
    bool ReadBlock(const char* data, const char*& cursor, const char* end, Word flags);
    
    Node rootNode;
    Node* currentNode;
};

}
}
}

#endif
