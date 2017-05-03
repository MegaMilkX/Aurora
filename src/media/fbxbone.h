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
        else
        {
            Au::Math::Vec3f pos;
            Node& lclTrans = node.Get("Properties70").GetWhere(0, "Lcl Translation");
            pos.x = (float)lclTrans[4].GetDouble();
            pos.y = (float)lclTrans[5].GetDouble();
            pos.z = (float)lclTrans[6].GetDouble();
            Au::Math::Vec3f rot;
            Node& lclRot = node.Get("Properties70").GetWhere(0, "Lcl Rotation");
            rot.x = (float)lclRot[4].GetDouble();
            rot.y = (float)lclRot[5].GetDouble();
            rot.z = (float)lclRot[6].GetDouble();
            Au::Math::Vec3f scale(1.0f, 1.0f, 1.0f);
            
            transform = 
                Au::Math::Translate(Au::Math::Mat4f(1.0f), pos) * 
                Au::Math::ToMat4(EulerToQuat(rot)) * 
                Au::Math::Scale(Au::Math::Mat4f(1.0f), scale);
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
