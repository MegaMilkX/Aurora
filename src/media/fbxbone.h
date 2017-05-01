#ifndef AU_FBXBONE_H
#define AU_FBXBONE_H

#include "../math/math.h"

#include "fbxnode.h"

namespace Au{
namespace Media{
namespace FBX{

class Bone
{
public:
    Bone(Node& root, Node& node)
    : transform(1.0f), uid(0), puid(0)
    {
        uid = node[0].GetInt64();
        name = node[1].GetString();
        std::cout << name << std::endl;
        std::cout << uid << std::endl;
        Node* conn;
        Node* deformer = 
            root.GetConnectedParent("Deformer", uid, &conn);
        Node* parent = 
            root.GetConnectedParent("Model", uid, &conn);
            
        if(parent)
            puid = (*parent)[0].GetInt64();
        
        if(deformer)
        {
            std::vector<double> mat =
                deformer->Get("Transform")[0].GetArray<double>();
            std::vector<float> matf(mat.size());
            for(unsigned i = 0; i < mat.size(); ++i)
                matf[i] = (float)mat[i];
            
            transform = *(Math::Mat4f*)matf.data();
        }
    }
    
    int64_t uid;
    int64_t puid;
    std::string name;
    Math::Mat4f transform;
}; 

}
}
}

#endif
