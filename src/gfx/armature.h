#ifndef SKELETON_H
#define SKELETON_H

#include <string>
#include <vector>

#include <stdint.h>

#include "../math/math.h"

namespace Au{
namespace GFX{

struct Bone
{
    int64_t uid;
    int64_t parentUID;
    Math::Mat4f bindTransform;
    std::string name;
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