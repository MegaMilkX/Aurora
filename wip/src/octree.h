#ifndef OCTREE_H
#define OCTREE_H

namespace Au{

class Object
{
    
};

class Cell
{
public:
    bool Insert(const Object& object)
    {
        
    }
private:
    Math::Vec3f pos;
    Math::Vec3f size;
    Cell* children[8];
};

class Octree
{
public:
    void Insert(const Object& object)
    {
        while(!rootCell->Insert(object))
            Expand(object);
    }
    void Expand(const Object& object)
    {
        
    }
private:
    Cell* rootCell;
};

int main()
{
    Octree octree;
    
    return 0;
}

}

#endif
