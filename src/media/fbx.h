#ifndef FBX_H
#define FBX_H

#include "fbxobject.h"
#include "fbxmesh.h"

namespace Au{
namespace Media{
namespace FBX{

typedef uint8_t Byte;
typedef uint32_t Word;
typedef uint64_t DoubleWord;

class Bone
{
public:
    int64_t uid;
    int64_t parentUID;
    std::string name;
    Math::Mat4f bindTransform;
    std::vector<uint64_t> indices;
    std::vector<float> weights;
};

class Reader
{
public:
    Reader()
    {}
    bool ReadFile(const char* data, unsigned size);
    
    void ConvertCoordSys(CoordSystem sys)
    { 
        coordSys = sys;
        if(sys == OPENGL)
            for(unsigned i = 0; i < meshes.size(); ++i)
                meshes[i].ConvertCoordSystem(AXIS_X, AXIS_Y, AXIS_Z);
        else if(sys == DIRECT3D)
            for(unsigned i = 0; i < meshes.size(); ++i)
                meshes[i].ConvertCoordSystem(AXIS_X, AXIS_Y, AXIS_MZ);
    }
    
    Axis GetUpAxis()
    {
        int axis = (int)rootNode.Get("Properties70", 0).GetWhere(0, "UpAxis")[4].GetInt32();
        Axis up = FBXAxisToAxis(axis);
        int sign = (int)rootNode.Get("Properties70", 0).GetWhere(0, "UpAxisSign")[4].GetInt32();
        if(sign != 1)
            FlipAxis(up);
        return up;
    }
    Axis GetFrontAxis()
    {
        int axis = (int)rootNode.Get("Properties70", 0).GetWhere(0, "FrontAxis")[4].GetInt32();
        Axis front = FBXAxisToAxis(axis);
        int sign = -(int)rootNode.Get("Properties70", 0).GetWhere(0, "FrontAxisSign")[4].GetInt32();
        if(sign != 1)
            FlipAxis(front);
        return front;
    }
    Axis GetRightAxis()
    {
        int axis = (int)rootNode.Get("Properties70", 0).GetWhere(0, "CoordAxis")[4].GetInt32();
        Axis right = FBXAxisToAxis(axis);
        int sign = (int)rootNode.Get("Properties70", 0).GetWhere(0, "CoordAxisSign")[4].GetInt32();
        if(sign != 1)
            FlipAxis(right);
        return right;
    }
    
    void FlipAxis(Axis& axis)
    {
        if(axis == AXIS_X)
            axis = AXIS_MX;
        else if(axis == AXIS_Y)
            axis = AXIS_MY;
        else if(axis == AXIS_Z)
            axis = AXIS_MZ;
        else if(axis == AXIS_MX)
            axis = AXIS_X;
        else if(axis == AXIS_MY)
            axis = AXIS_Y;
        else if(axis == AXIS_MZ)
            axis = AXIS_Z;
    }
    
    int MeshCount() { return meshes.size(); }
    Mesh& GetMesh(unsigned id) { return meshes[id]; }
    
    std::vector<Bone> GetBones();
    
    void Print()
    {
        rootNode.Print();
    }
private:
    void ReadData(Prop& prop, std::vector<char>& out, const char* data, const char*& cursor, const char* end);
    bool ReadBlock(Node& node, const char* data, const char*& cursor, const char* end, Word flags);
    bool ReadFileFBX(const char* data, unsigned size);
    
    bool ReadVerticesAndIndices(Mesh& mesh, unsigned meshId);
    bool ReadNormals(Mesh& mesh, unsigned meshId);
    bool ReadUV(Mesh& mesh, unsigned meshId);
    bool ReadWeights(Mesh& mesh, unsigned meshId);
    bool ReadSkin(Mesh& mesh, unsigned meshId);
    
    std::vector<Node> GetConnectedChildren(const std::string& childName, Node& node)
    {
        std::vector<Node> result;
        std::vector<Node> connections = 
            rootNode.GetNodesWithProp("C", 2, node[0].GetInt64());
        for(unsigned i = 0; i < connections.size(); ++i)
        {
            Node* n = rootNode.GetNodeWithUID(childName, connections[i][1].GetInt64());
            if(n)
                result.push_back(*n);
        }
        return result;
    }
    
    Axis FBXAxisToAxis(unsigned axis)
    { 
        if(axis == 0)
            return AXIS_X;
        else if(axis == 1)
            return AXIS_Z;
        else if(axis == 2)
            return AXIS_Y;
        else
            return AXIS_UNKNOWN;
    }
    
    Node rootNode;
    CoordSystem coordSys;
    std::vector<Mesh> meshes;
};

}
}
}

#endif
