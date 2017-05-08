#ifndef AU_FBXBONE_H
#define AU_FBXBONE_H

#include "../math/math.h"

#include "fbxnode.h"

#include "fbxutil.h"

#include "fbxpose.h"

namespace Au{
namespace Media{
namespace FBX{

class Bone
{
public:
    Bone(Settings& settings, Node& root, Node& node)
    : transform(1.0f), uid(0), puid(0)
    {
        uid = node[0].GetInt64();
        name = node[1].GetString();
        
        Node* conn;
        Node* deformer = 
            root.GetConnectedParent("Deformer", uid, &conn);
        Node* parent = 
            root.GetConnectedParent("Model", uid, &conn);
            
        Node* poseData = root.GetWhere("Pose", 2, "BindPose");
        Pose pose(settings, &root, poseData);
        
        if(parent)
            puid = (*parent)[0].GetInt64();
        
        Au::Math::Vec3f pos(0.0f, 0.0f, 0.0f);
        Au::Math::Quat qrot(0.0f, 0.0f, 0.0f, 1.0f);
        Au::Math::Vec3f scale(1.0f, 1.0f, 1.0f);
        /*
        if(deformer)
        {
            std::vector<double> mat =
                deformer->Get("TransformLink")[0].GetArray<double>();
            if(mat.size() < 16)
                return;
            std::vector<float> matf(mat.size());
            for(unsigned i = 0; i < mat.size(); ++i)
                matf[i] = (float)mat[i];
            
            Au::Math::Mat4f mat4f = *(Math::Mat4f*)matf.data();
            
            ConvertMatrix(settings, mat4f);
            
            transform = mat4f;
        }*/
        
        if(!pose.GetPoseTransform(uid, transform))
        {
            SceneNode sn(settings, node);

            pos = sn.LclTranslation();            
            qrot = Au::Math::EulerToQuat(sn.LclRotation());
            scale = sn.LclScaling();
            
            transform = 
                Au::Math::Translate(Au::Math::Mat4f(1.0f), pos) * 
                Au::Math::ToMat4(qrot) * 
                Au::Math::Scale(Au::Math::Mat4f(1.0f), scale);
        }
        
        
    }
    
    int64_t uid;
    int64_t puid;
    std::string name;
    Au::Math::Mat4f transform;
}; 

}
}
}

#endif
