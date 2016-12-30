#ifndef QUADTREE_H
#define QUADTREE_H

template<typename T, typename UNIT = float>
class Octree
{
public:
    void Insert(UNIT x, UNIT y, UNIT z, const T& value);
    void Erase(UNIT x, UNIT y, UNIT z);
};

#endif
