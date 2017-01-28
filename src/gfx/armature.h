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
    Bone* GetBone(int64_t uid)
    {
        for(unsigned i = 0; i < bones.size(); ++i)
            if(bones[i].uid == uid)
                return &(bones[i]);
            
        Bone bone;
        bone.uid = uid;
        bones.push_back(bone);
        return &(bones[bones.size() - 1]);
    }
private:
    std::vector<Bone> bones;
    std::vector<std::string> boneNames;
};

}
}

#endif
