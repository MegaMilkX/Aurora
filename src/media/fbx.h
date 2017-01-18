#ifndef FBX_H
#define FBX_H

#include <string>
#include <vector>

namespace Au{
namespace Media{
namespace FBX{

//bool Read(const char* data, unsigned size);

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
