#ifndef AU_FBXANIMSTACK_H
#define AU_FBXANIMSTACK_H

#include "fbxnode.h"

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
    
class AnimationCurve
{
public:
    AnimationCurve(Node& root, Node& node, const std::string& name)
    : name(name)
    {
        std::vector<int64_t> keyTime =
            node.Get("KeyTime")[0].GetArray<int64_t>();
        std::vector<float> keyValue = 
            node.Get("KeyValueFloat")[0].GetArray<float>();
            
        for(unsigned i = 0; i < keyTime.size() && i < keyValue.size(); ++i)
        {
            //int frameTime = 46186158000 / FPS;
            //int frame = keyTime[i] / frameTime;
            //std::cout << keyTime[i] << ": " << keyValue[i] << std::endl;
            keyframes.push_back(Keyframe(keyTime[i], keyValue[i]));
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
    AnimationCurveNode(Node& root, Node& node)
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
            curves.push_back(AnimationCurve(root, *(nodes[i]), connName));
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
    AnimationLayer(Node& root, Node& object)
    {
        int64_t uid = object[0].GetInt64();
        
        std::vector<Node*> nodes = 
            root.GetConnectedChildren("AnimationCurveNode", uid);
        for(unsigned i = 0; i < nodes.size(); ++i)
            curveNodes.push_back(AnimationCurveNode(root, *(nodes[i])));
    }
    
    unsigned CurveNodeCount() { return curveNodes.size(); }
    AnimationCurveNode* GetCurveNode(unsigned id) { return &curveNodes[id]; }
private:
    std::vector<AnimationCurveNode> curveNodes;
};

class AnimationStack
{
public:
    AnimationStack(Node& root, Node& object)
    {
        int64_t uid = object[0].GetInt64();
        name = object[1].GetString();
        
        std::vector<Node*> nodes =
            root.GetConnectedChildren("AnimationLayer", uid);
        for(unsigned i = 0; i < nodes.size(); ++i)
            layers.push_back(AnimationLayer(root, *(nodes[i])));
        
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
