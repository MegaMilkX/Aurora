#ifndef AU_FBXANIMSTACK_H
#define AU_FBXANIMSTACK_H

#include "fbxnode.h"

namespace Au{
namespace Media{
namespace FBX{
    
class AnimationCurve
{
public:
    AnimationCurve(Node& root, Node& node, const std::string& name)
    : name(name)
    {
        std::cout << node.Name() << std::endl;
        std::cout << name << std::endl;
        std::vector<int64_t> keyTime =
            node.Get("KeyTime")[0].GetArray<int64_t>();
        std::vector<float> keyValue = 
            node.Get("KeyValueFloat")[0].GetArray<float>();
            
        for(unsigned i = 0; i < keyTime.size() && i < keyValue.size(); ++i)
        {
            std::cout << keyTime[i] << ": " << keyValue[i] << std::endl;
        }
    }
private:
    std::string name;
};
    
class AnimationCurveNode
{
public:
    AnimationCurveNode(Node& root, Node& node)
    {
        std::cout << node.Name() << std::endl;
        int64_t uid = node[0].GetInt64();
        
        Node* conn = 0;
        Node* n = root.GetConnectedParent("Model", uid, &conn);
        if(n && conn)
        {
            objectName = (*n)[1].GetString();
            propName = (*conn)[3].GetString();
        }
        
        std::cout << objectName << std::endl;
        std::cout << propName << std::endl;
        
        std::vector<Node*> conns;
        std::vector<Node*> nodes = 
            root.GetConnectedChildren("AnimationCurve", uid, conns);
        for(unsigned i = 0; i < nodes.size() && i < conns.size(); ++i)
        {
            std::string connName = (*conns[i])[3].GetString();
            curves.push_back(AnimationCurve(root, *(nodes[i]), connName));
        }
    }
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
        std::cout << object.Name() << std::endl;
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
        std::cout << object.Name() << std::endl;
        int64_t uid = object[0].GetInt64();
        
        std::vector<Node*> nodes =
            root.GetConnectedChildren("AnimationLayer", uid);
        for(unsigned i = 0; i < nodes.size(); ++i)
            layers.push_back(AnimationLayer(root, *(nodes[i])));
    }
    
    unsigned LayerCount() { return layers.size(); }
    AnimationLayer* GetLayer(unsigned id) { return &layers[id]; }
private:
    std::vector<AnimationLayer> layers;
};

}
}
}

#endif
