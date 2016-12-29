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
    
    for(unsigned int i = 0; i < meshes.size(); ++i)
    {
        FbxMesh* mesh = meshes[i];
        if(!mesh->IsTriangleMesh())
        {
            //continue;
            // ??
        }
        
        FbxVector4* vertices = mesh->GetControlPoints();
        FbxLayer* layer = mesh->GetLayer(0);
        FbxLayerElementNormal* normals = layer->GetNormals();
        FbxLayerElementUV* uvElement = layer->GetUVs();
        FbxLayerElementArrayTemplate<FbxVector2> uvs = uvElement->GetDirectArray();
        
        unsigned long nV = mesh->GetControlPointsCount();
        for (unsigned int v = 0; v < nV; ++v)
        {
            //vertices[v].mData[0];
            //vertices[v].mData[2];
            //-vertices[v].mData[1];
            
            
        }
    }
}

std::vector<MeshObject> LoadFromFBX(const File& file)
{
    
}

std::vector<MeshObject> LoadFromFBX(const void* data, size_t sz)
{
    
}

}
}
