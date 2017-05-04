#ifndef AU_FBXBONE_H
#define AU_FBXBONE_H

#include "../math/math.h"

#include "fbxnode.h"

#include "fbxsettings.h"

namespace Au{
namespace Media{
namespace FBX{
    
inline bool IsSameAxisDirection(Axis a, Axis b)
{
    if(a == AXIS_X && b == AXIS_X)
        return true;
    else if(a == AXIS_Y && b == AXIS_Y)
        return true;
    else if(a == AXIS_Z && b == AXIS_Z)
        return true;
    return false;
}
    
inline bool IsSameAxis(Axis a, Axis b)
{
    if((a == AXIS_X || a == AXIS_MX) &&
       (b == AXIS_X || b == AXIS_MX))
        return true;
    else if((a == AXIS_Y || a == AXIS_MY) &&
       (b == AXIS_Y || b == AXIS_MY))
       return true;
    else if((a == AXIS_Z || a == AXIS_MZ) &&
       (b == AXIS_Z || b == AXIS_MZ))
       return true;
    else
        return false;
}
    
inline void GetConvertedAxisIndices(Settings& settings, unsigned (&indices)[3], float (&mults)[3])
{
    Axis origAxes[3] = { 
        settings.origAxes.right,
        settings.origAxes.up,
        settings.origAxes.front
    };
    Axis convAxes[3] = {
        settings.convAxes.right,
        settings.convAxes.up,
        settings.convAxes.front
    };
    
    for(unsigned i = 0; i < 3; ++i)
    {
        for(unsigned j = 0; j < 3; ++j)
        {
            if(IsSameAxis(origAxes[i], convAxes[j]))
            {
                indices[i] = j;
                if(!IsSameAxisDirection(origAxes[i], convAxes[j]))
                    mults[i] = -1.0f;
            }
        }
    }
}
    
inline void ConvertVector(Settings& settings, Au::Math::Vec3f& vec)
{
    unsigned indices[3];
    float multipliers[3] = { 1.0f, 1.0f, 1.0f };
    
    GetConvertedAxisIndices(settings, indices, multipliers);
    
    float newVec[3];
    for(unsigned i = 0; i < 3; ++i)
        newVec[indices[i]] = vec[i] * multipliers[i];
    vec.x = newVec[0];
    vec.y = newVec[1];
    vec.z = newVec[2];
}

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
            
        if(parent)
            puid = (*parent)[0].GetInt64();
        
        Au::Math::Vec3f pos(0.0f, 0.0f, 0.0f);
        Au::Math::Vec3f rot(0.0f, 0.0f, 0.0f);
        Au::Math::Quat qrot(0.0f, 0.0f, 0.0f, 1.0f);
        Au::Math::Vec3f scale(1.0f, 1.0f, 1.0f);
        
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
            
            pos = Au::Math::Vec3f(matf[12], matf[13], matf[14]);
            qrot = Au::Math::ToQuat(ToMat3(mat4f));
            
            //ConvertVector(settings, pos);
            
            transform = 
                Au::Math::Translate(Au::Math::Mat4f(1.0f), pos) * 
                Au::Math::ToMat4(qrot) * 
                Au::Math::Scale(Au::Math::Mat4f(1.0f), scale);
        }
        else
        {
            Node& lclTrans = node.Get("Properties70").GetWhere(0, "Lcl Translation");
            pos.x = (float)lclTrans[4].GetDouble();
            pos.y = (float)lclTrans[5].GetDouble();
            pos.z = (float)lclTrans[6].GetDouble();
            
            ConvertVector(settings, pos);
            
            Node& lclRot = node.Get("Properties70").GetWhere(0, "Lcl Rotation");
            rot.x = (float)lclRot[4].GetDouble();
            rot.y = (float)lclRot[5].GetDouble();
            rot.z = (float)lclRot[6].GetDouble();
            
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
