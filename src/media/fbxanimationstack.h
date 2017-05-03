#ifndef AU_FBXANIMSTACK_H
#define AU_FBXANIMSTACK_H

#include "fbxnode.h"

#include "fbxsettings.h"

namespace Au{
namespace Media{
namespace FBX{

const int64_t TimeSecond = 46186158000;
    
enum TIME_MODE
{
    FRAMES_DEFAULT,
    FRAMES_120,
    FRAMES_100,
    FRAMES_60,
    FRAMES_50,
    FRAMES_48,
    FRAMES_30,
    FRAMES_30_DROP,
    FRAMES_NTSC_DROP,
    FRAMES_NTSC_FULL,
    FRAMES_PAL,
    FRAMES_CINEMA,
    FRAMES_1000m,
    FRAMES_CINEMA_ND,
    FRAMES_CUSTOM,
    FRAMES_96,
    FRAMES_72,
    FRAMES_59dot94,
    FRAMES_MODE_COUNT
};
    
struct Keyframe
{
    Keyframe(int64_t frame, float value)
    : frame(frame), value(value) {}
    int64_t frame;
    float value;
};

inline std::string AxisToFBXName(Axis axis)
{
    static std::vector<std::string> axisName =
        { "d|X", "d|Y", "d|Z" };
        
    int a = (int)axis;
    if(a < 0)
        a = -a - 1;
    
    return axisName[a];
}

inline std::string DirToFBXAxis(AxisSetup axes, int dir)
{
    std::vector<std::string> axisName =
        { "" };
        
    axisName.push_back(AxisToFBXName(axes.right));
    axisName.push_back(AxisToFBXName(axes.up));
    axisName.push_back(AxisToFBXName(axes.front));
    
    return axisName[abs(dir)];
}

inline bool EqualsFBXAxis(Axis axis, const std::string& name)
{
    if(AxisToFBXName(axis) == name)
        return true;
    return false;
}

inline int DirFromFBXAxis(AxisSetup axes, const std::string& name)
{
    int dir = 0;
    if(EqualsFBXAxis(axes.right, name))
        if((int)(axes.right) < 0)
            dir = -1;
        else
            dir = 1;
    else if(EqualsFBXAxis(axes.up, name))
        if((int)(axes.up) < 0)
            dir = -2;
        else
            dir = 2;
    else if(EqualsFBXAxis(axes.front, name))
        if((int)(axes.front) < 0)
            dir = -3;
        else
            dir = 3;
    return dir;
}

inline void ConvertCurveName(Settings& settings, std::string& name)
{
    int origDir = DirFromFBXAxis(settings.origAxes, name);
    int convDir = DirFromFBXAxis(settings.convAxes, name);

    name = DirToFBXAxis(settings.origAxes, convDir);
}

inline void ConvertCurveMult(Settings& settings, const std::string& name, float& valMult)
{
    int origDir = DirFromFBXAxis(settings.origAxes, name);
    int convDir = DirFromFBXAxis(settings.convAxes, name);
    
    if((origDir < 0) != (convDir < 0))
        valMult = -1.0f;
}
    
class AnimationCurve
{
public:
    AnimationCurve(
        Node& root, 
        Node& node, 
        const std::string& name, 
        const std::string& curveNodeName,
        Settings& settings
        )
    : name(name)
    {
        std::vector<int64_t> keyTime =
            node.Get("KeyTime")[0].GetArray<int64_t>();
        std::vector<float> keyValue = 
            node.Get("KeyValueFloat")[0].GetArray<float>();
        
        float valMult = 1.0f;
        if(/*curveNodeName.compare(0, 15, "Lcl Translation") == 0 ||*/
            curveNodeName.compare(0, 12, "Lcl Rotation") == 0)
        {
            ConvertCurveMult(settings, this->name, valMult);
        }
        
        if(curveNodeName.compare(0, 11, "Lcl Scaling") == 0 ||
            curveNodeName.compare(0, 12, "Lcl Rotation") == 0)
        {
            ConvertCurveName(settings, this->name);
        }
            
        for(unsigned i = 0; i < keyTime.size() && i < keyValue.size(); ++i)
        {
            //int frameTime = 46186158000 / FPS;
            //int frame = keyTime[i] / frameTime;
            //std::cout << keyTime[i] << ": " << keyValue[i] << std::endl;
            
            float val = keyValue[i] * valMult;
            if(curveNodeName.compare(0, 11, "Lcl Scaling") == 0)
            {
                //std::cout << "hello" << std::endl;
                //std::cout << settings.scaleFactor << std::endl;
                //val = (float)(val / settings.scaleFactor);
            }
            else if(curveNodeName.compare(0, 12, "Lcl Rotation") == 0)
            {
                val = (val * Au::Math::PI) / 180.0f;
            }
            keyframes.push_back(Keyframe(keyTime[i], val));
        }
    }
    
    std::string& GetName() { return name; }
    unsigned KeyframeCount() { return keyframes.size(); }
    Keyframe* GetKeyframe(unsigned id) { return &keyframes[id]; }
private:
    std::string name;
    std::vector<Keyframe> keyframes;
};
    
class AnimationCurveNode
{
public:
    AnimationCurveNode(Node& root, Node& node, Settings& settings)
    {
        int64_t uid = node[0].GetInt64();
        
        Node* conn = 0;
        Node* n = root.GetConnectedParent("Model", uid, &conn);
        if(n && conn)
        {
            objectName = (*n)[1].GetString();
            propName = (*conn)[3].GetString();
        }
        
        std::vector<Node*> conns;
        std::vector<Node*> nodes = 
            root.GetConnectedChildren("AnimationCurve", uid, conns);
        for(unsigned i = 0; i < nodes.size() && i < conns.size(); ++i)
        {
            std::string connName = (*conns[i])[3].GetString();
            curves.push_back(AnimationCurve(root, *(nodes[i]), connName, propName, settings));
        }
    }
    
    std::string& GetObjectName() { return objectName; }
    std::string& GetPropertyName() { return propName; }
    unsigned CurveCount() { return curves.size(); }
    AnimationCurve* GetCurve(unsigned id) { return &curves[id]; }
private:
    std::vector<AnimationCurve> curves;
    std::string objectName;
    std::string propName;
};

class AnimationLayer
{
public:
    AnimationLayer(Node& root, Node& object, Settings& settings)
    {
        int64_t uid = object[0].GetInt64();
        
        std::vector<Node*> nodes = 
            root.GetConnectedChildren("AnimationCurveNode", uid);
        for(unsigned i = 0; i < nodes.size(); ++i)
            curveNodes.push_back(AnimationCurveNode(root, *(nodes[i]), settings));
    }
    
    unsigned CurveNodeCount() { return curveNodes.size(); }
    AnimationCurveNode* GetCurveNode(unsigned id) { return &curveNodes[id]; }
private:
    std::vector<AnimationCurveNode> curveNodes;
};

class AnimationStack
{
public:
    AnimationStack(Node& root, Node& object, Settings& settings)
    {
        int64_t uid = object[0].GetInt64();
        name = object[1].GetString();
        
        std::vector<Node*> nodes =
            root.GetConnectedChildren("AnimationLayer", uid);
        for(unsigned i = 0; i < nodes.size(); ++i)
            layers.push_back(AnimationLayer(root, *(nodes[i]), settings));
        
        Node& prop70 = object.Get("Properties70");
        int pCount = prop70.Count("P");
        for(int i = 0; i < pCount; ++i)
        {
            Node& p = prop70.Get("P", i);
            std::string pName = p[0].GetString();
            if(pName == "LocalStop" ||
                pName == "ReferenceStop")
            {
                length = p[4].GetInt64();
                if(length)
                    break;
            }
        }
    }
    
    int64_t GetLength() { return length; }
    std::string GetName() { return name; }
    unsigned LayerCount() { return layers.size(); }
    AnimationLayer* GetLayer(unsigned id) { return &layers[id]; }
private:
    std::string name;
    std::vector<AnimationLayer> layers;
    int64_t length;
};

}
}
}

#endif
