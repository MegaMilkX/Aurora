#ifndef SKELETON_H
#define SKELETON_H

#include <string>
#include <vector>

#include "../math/math.h"

namespace Au{
namespace GFX{

class Bone
{
public:
private:
    unsigned parent;
    std::vector<unsigned> children;
};

class Armature
{
public:
private:
    std::vector<Bone> bones;
    std::vector<std::string> boneNames;
};

}
}

#endif
