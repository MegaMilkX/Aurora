#ifndef FBX_H
#define FBX_H

#include "fbxobject.h"
#include "fbxmesh.h"
#include "fbxanimationstack.h"

#include "fbxsettings.h"

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
    
    void ConvertCoordSys(CoordSystem sys);
    
    Axis GetUpAxis();
    Axis GetFrontAxis();
    Axis GetRightAxis();
    
    TIME_MODE GetTimeMode();
    int GetTimeProtocol();
    double GetCustomFrameRate();
    double GetFrameRate();
    
    std::vector<AnimationStack> GetAnimationStacks();
    
    void FlipAxis(Axis& axis);
    
    int MeshCount() { return meshes.size(); }
    Mesh& GetMesh(unsigned id) { return meshes[id]; }
    
    std::vector<Bone> GetBones();
    
    Node& GetRootNode() { return rootNode; }
    
    void Print() { rootNode.Print(); }
private:
    void ReadData(Prop& prop, std::vector<char>& out, const char* data, const char*& cursor, const char* end);
    bool ReadBlock(Node& node, const char* data, const char*& cursor, const char* end, Word flags);
    bool ReadFileFBX(const char* data, unsigned size);
    
    bool ReadVerticesAndIndices(Mesh& mesh, unsigned meshId);
    bool ReadNormals(Mesh& mesh, unsigned meshId);
    bool ReadUV(Mesh& mesh, unsigned meshId);
    bool ReadWeights(Mesh& mesh, unsigned meshId);
    bool ReadSkin(Mesh& mesh, unsigned meshId);
    
    std::vector<Node> GetConnectedChildren(const std::string& childName, Node& node);
    
    Axis FBXAxisToAxis(unsigned axis);
    
    Settings settings;
    Node rootNode;
    CoordSystem coordSys;
    std::vector<Mesh> meshes;
};

}
}
}

#endif
