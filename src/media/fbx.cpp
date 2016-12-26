#include "fbx.h"

namespace Au{
namespace Media{

void GetMeshes(FbxNode* node, std::vector<FbxMesh*> &meshes)
{
    if (node->GetNodeAttribute())
    {
        if (node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
        {
            meshes.push_back((FbxMesh*)node->GetNodeAttribute());
        }
    }

    for (int i = 0; i < node->GetChildCount(); ++i)
    {
        GetMeshes(node->GetChild(i), meshes);
    }
}

std::vector<MeshObject> LoadFromFBX(const std::string& filename)
{
    std::vector<MeshObject> objects;
    
    FbxManager* sdkManager = FbxManager::Create();
    FbXIOSettings* ios = FbxIOSettings::Create(sdkManager, IOSROOT);
    sdkManager->SetIOSettings(ios);
    
    FbxImporter* importer = FbxImporter::Create(sdkManager, "");
    
    bool importOk = importer->Initialize(filename.c_str(), -1, sdkManager->GetIOSettings());
    
    if(!importOk)
    {
        return objects;
    }
    
    FbxScene* scene = FbxScene::Create(sdkManager, "scene");
    importer->Import(scene);
    importer->Destroy();
    
    std::vector<FbxMesh*> meshes;
    FbxNode* node = scene->GetRootNode();
    GetMeshes(node, meshes);
}

std::vector<MeshObject> LoadFromFBX(const File& file)
{
    
}

std::vector<MeshObject> LoadFromFBX(const void* data, size_t sz)
{
    
}

}
}
