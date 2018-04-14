#ifndef AU_FBXMESH_H
#define AU_FBXMESH_H

#include <vector>
#include <string>

#include "fbxsettings.h"

#include "fbxnode.h"
#include "fbxmodel.h"

#include "fbxskin.h"

#include "fbxpose.h"

namespace Au{
namespace Media{
namespace FBX{

class Mesh
{
public:
    Mesh() {}
    Mesh(Settings& settings, Node& rootNode, Node& geom)
    {
        uid = geom[0].GetInt64();
        
        Node* conn = 0;
        Node* model = 
            rootNode.GetConnectedParent("Model", uid, &conn);
        if(model)
        {
            name = (*model)[1].GetString();
        }
        
        Math::Mat4f bindTransform = Math::Mat4f(1.0f);
        Node* poseData = rootNode.GetWhere("Pose", 2, "BindPose");
        Pose pose(settings, &rootNode, poseData);
        
        if(!pose.GetPoseTransform((*model)[0].GetInt64(), bindTransform))
        {
            Model model(&settings, &rootNode, model);
            bindTransform = model.transform;
        }
        
        _getVerticesAndIndices(settings, rootNode, geom, bindTransform);
        _getNormals(settings, rootNode, geom, bindTransform);
        _getUV(rootNode, geom);
        
        OptimizeDuplicates();
        
        skin = Skin(rootNode, geom);
    }
    
    int VertexCount() { return vertices.size() / 3; }
    
    std::vector<float> GetVertices()
    {
        std::vector<float> result = vertices;
        return result;
    }
    std::vector<float> GetNormals(unsigned layer)
    {
        if(layer >= normalLayers.size())
            return std::vector<float>();
        return normalLayers[layer];
    }
    std::vector<float> GetUV(unsigned layer)
    {
        if(layer >= uvLayers.size())
            return std::vector<float>();
        return uvLayers[layer];
    }
    template<typename T>
    std::vector<T> GetIndices()
    {
        std::vector<T> result;
        for(unsigned i = 0; i < indices.size(); ++i)
            result.push_back((T)indices[i]);
        return result;
    }
    
    Skin& GetSkin()
    {
        return skin;
    }
    
    void ConvertCoordSystem(Axis new_right, Axis new_up, Axis new_front)
    {
        ConvertVertexArray3(vertices, right, up, front,
                            new_right, new_up, new_front);
        for(unsigned i = 0; i < normalLayers.size(); ++i)
            ConvertVertexArray3(normalLayers[i], right, up, front,
                                new_right, new_up, new_front);
        
        this->right = new_right;
        this->up = new_up;
        this->front = new_front;
    }
    
    template<typename T>
    void ConvertVertexArray3(std::vector<T>& data,
                            Axis right, Axis up, Axis front,
                            Axis new_right, Axis new_up, Axis new_front)
    {
        for(unsigned i = 0; i < data.size(); i += 3)
        {
            int right_sign = right >= 0 && new_right >= 0 ||
                            right < 0 && new_right < 0 ?
                            1 : -1;
                            
            int up_sign = up >= 0 && new_up >= 0 ||
                            up < 0 && new_up < 0 ?
                            1 : -1;
                            
            int front_sign = front >= 0 && new_front >= 0 ||
                            front < 0 && new_front < 0 ?
                            1 : -1;
            
            T r = right >= 0 ? data[i + right] : data[i - (right + 1)];
            T u = up >= 0 ? data[i + up] : data[i - (up + 1)];
            T f = front >= 0 ? data[i + front] : data[i - (front + 1)];
            
            new_right = (Axis)(new_right < 0 ? -(new_right + 1) : new_right);
            new_up = (Axis)(new_up < 0 ? -(new_up + 1) : new_up);
            new_front = (Axis)(new_front < 0 ? -(new_front + 1) : new_front);
            
            data[i + new_right] = r * right_sign;
            data[i + new_up] = u * up_sign;
            data[i + new_front] = f * front_sign;
        }
    }

    void OptimizeDuplicates()
    {
        for(unsigned i = 0; i < indices.size(); ++i)
            for(unsigned j = i + 1; j < indices.size(); ++j)
                if(IsSameVertex(indices[i], indices[j]))
                    EraseVertex(indices[j], indices[i]);
    }
    
    bool IsSameVertex(unsigned a, unsigned b)
    {
        if(a == b)
            return false;
        
        if(vertices[a * 3] != vertices[b * 3] || 
           vertices[a * 3 + 1] != vertices[b * 3 + 1] ||
           vertices[a * 3 + 2] != vertices[b * 3 + 2])
            return false;
        else
        {
            for(unsigned i = 0; i < normalLayers.size(); ++i)
            {
                if(normalLayers[i][a * 3] != normalLayers[i][b * 3] || 
                   normalLayers[i][a * 3 + 1] != normalLayers[i][b * 3 + 1] ||
                   normalLayers[i][a * 3 + 2] != normalLayers[i][b * 3 + 2])
                    return false;
            }
            
            for(unsigned i = 0; i < uvLayers.size(); ++i)
            {
                if(uvLayers[i][a * 2] != uvLayers[i][b * 2] || 
                   uvLayers[i][a * 2 + 1] != uvLayers[i][b * 2 + 1])
                    return false;
            }
            
            for(unsigned i = 0; i < rgbaLayers.size(); ++i)
            {
                if(rgbaLayers[i][a * 4] != rgbaLayers[i][b * 4] || 
                   rgbaLayers[i][a * 4 + 1] != rgbaLayers[i][b * 4 + 1] ||
                   rgbaLayers[i][a * 4 + 2] != rgbaLayers[i][b * 4 + 2] ||
                   rgbaLayers[i][a * 4 + 3] != rgbaLayers[i][b * 4 + 3])
                    return false;
            }
        }
        
        return true;
    }
    
    void EraseVertex(int id, int newId)
    {
        for(unsigned i = 0; i < indices.size(); ++i)
            if(indices[i] == id)
                indices[i] = newId;
            
        for(unsigned i = 0; i < indices.size(); ++i)
            if(indices[i] >= id)
                indices[i]--;
            
        vertices.erase(vertices.begin() + id * 3, vertices.begin() + id * 3 + 3);
        origVertIndices.erase(origVertIndices.begin() + id);
        
        for(unsigned i = 0; i < normalLayers.size(); ++i)
            normalLayers[i].erase(normalLayers[i].begin() + id * 3, normalLayers[i].begin() + id * 3 + 3);
        
        for(unsigned i = 0; i < uvLayers.size(); ++i)
            uvLayers[i].erase(uvLayers[i].begin() + id * 2, uvLayers[i].begin() + id * 2 + 2);
        
        for(unsigned i = 0; i < rgbaLayers.size(); ++i)
            rgbaLayers[i].erase(rgbaLayers[i].begin() + id * 4, rgbaLayers[i].begin() + id * 4 + 4);
    }
    
    int64_t uid;

    CoordSystem coordSys;
    Axis right, up, front;
    
    std::string name;
    
    std::vector<int> indices;
    std::vector<float> vertices;
    std::vector<int32_t> origVertIndices;
    std::vector<std::vector<float>> normalLayers;
    std::vector<std::vector<float>> uvLayers;
    std::vector<std::vector<unsigned char>> rgbaLayers;
    
    Skin skin;
private:
    void _getVerticesAndIndices(Settings& settings, Node& rootNode, Node& geom, const Math::Mat4f& transform)
    {        
        std::vector<int32_t> fbxIndices = 
            geom.Get("PolygonVertexIndex")[0].GetArray<int32_t>();
        std::vector<float> fbxVertices =
            geom.Get("Vertices")[0].GetArray<float>();
            
        for(unsigned j = 0; j < fbxIndices.size(); ++j)
        {
            int32_t idx = fbxIndices[j] < 0 ? -fbxIndices[j] - 1 : fbxIndices[j];
            Au::Math::Vec3f vert(
                fbxVertices[idx * 3] * (float)settings.scaleFactor,
                fbxVertices[idx * 3 + 1] * (float)settings.scaleFactor,
                fbxVertices[idx * 3 + 2] * (float)settings.scaleFactor
            );
            vert = transform * Au::Math::Vec4f(vert.x, vert.y, vert.z, 1.0f);
            vertices.push_back(vert.x);
            vertices.push_back(vert.y);
            vertices.push_back(vert.z);
            indices.push_back(j);
            origVertIndices.push_back(idx);
        }
    }
    
    void _getNormals(Settings& settings, Node& rootNode, Node& geom, const Math::Mat4f& transform)
    {
        std::vector<int32_t> fbxIndices = 
            geom.Get("PolygonVertexIndex")[0].GetArray<int32_t>();
        
        int normalLayerCount = geom.Count("LayerElementNormal");
        for(int j = 0; j < normalLayerCount; ++j)
        {
            Node& layerElemNormal = geom.Get("LayerElementNormal", j);
            std::vector<float> fbxNormals = 
                layerElemNormal.Get("Normals")[0].GetArray<float>();
            std::string normalsMapping = 
                layerElemNormal.Get("MappingInformationType")[0].GetString();
            
            std::vector<float> normals;
            
            if(normalsMapping == "ByVertex" || normalsMapping == "ByVertice")
            {
                for(unsigned l = 0; l < fbxIndices.size(); ++l)
                {
                    int32_t idx = fbxIndices[l] < 0 ? -fbxIndices[l] - 1 : fbxIndices[l];
                    Au::Math::Vec3f norm(
                        fbxNormals[idx * 3],
                        fbxNormals[idx * 3 + 1],
                        fbxNormals[idx * 3 + 2]
                    );
                    norm = transform * Au::Math::Vec4f(norm.x, norm.y, norm.z, 0.0f);
                    normals.push_back(norm.x);
                    normals.push_back(norm.y);
                    normals.push_back(norm.z);
                }
            }
            else if(normalsMapping == "ByPolygon")
            {
                normals = std::vector<float>(vertices.size());
                
                unsigned index = 0;
                for(unsigned k = 0; k < fbxNormals.size() / 3; ++k)
                {
                    std::vector<unsigned> polyIndices;
                    for(unsigned l = index; l < fbxIndices.size(); ++l)
                        polyIndices.push_back(l);
                    
                    for(unsigned l = 0; l < polyIndices.size(); ++l)
                    {
                        Au::Math::Vec3f norm(
                            fbxNormals[k * 3],
                            fbxNormals[k * 3 + 1],
                            fbxNormals[k * 3 + 2]
                        );
                        norm = transform * Au::Math::Vec4f(norm.x, norm.y, norm.z, 0.0f);
                        normals[polyIndices[l] * 3] = norm.x;
                        normals[polyIndices[l] * 3 + 1] = norm.y;
                        normals[polyIndices[l] * 3 + 2] = norm.z;
                    }
                }
            }
            else if(normalsMapping == "ByPolygonVertex")
            {
                normals = std::vector<float>(vertices.size());
                
                for(unsigned k = 0; k < fbxIndices.size(); ++k)
                {
                    int32_t idx = fbxIndices[k] < 0 ? -fbxIndices[k] - 1 : fbxIndices[k];
                    Au::Math::Vec3f norm(
                        fbxNormals[k * 3],
                        fbxNormals[k * 3 + 1],
                        fbxNormals[k * 3 + 2]
                    );
                    norm = transform * Au::Math::Vec4f(norm.x, norm.y, norm.z, 0.0f);
                    normals[k * 3] = norm.x;
                    normals[k * 3 + 1] = norm.y;
                    normals[k * 3 + 2] = norm.z;
                }
            }
            
            normalLayers.push_back(normals);
        }
    }
    
    void _getUV(Node& rootNode, Node& geom)
    {
        std::vector<int32_t> fbxIndices = 
            geom.Get("PolygonVertexIndex")[0].GetArray<int32_t>();
            
        int layerCount = geom.Count("LayerElementUV");
        for(int i = 0; i < layerCount; ++i)
        {
            Node& layer = geom.Get("LayerElementUV", i);
            std::vector<float> fbxUV =
                layer.Get("UV")[0].GetArray<float>();
            std::vector<int32_t> fbxUVIndex =
                layer.Get("UVIndex")[0].GetArray<int32_t>();
            std::string mapping =
                layer.Get("MappingInformationType")[0].GetString();
            std::string refType =
                layer.Get("ReferenceInformationType")[0].GetString();
            
            std::vector<float> uv;
            
            if(mapping == "ByVertex" || mapping == "ByVertice")
            {
                for(unsigned j = 0; j < fbxIndices.size(); ++j)
                {
                    int32_t idx = fbxIndices[j] < 0 ? -fbxIndices[j] - 1 : fbxIndices[j];
                    
                    uv.push_back(fbxUV[idx * 2]);
                    uv.push_back(fbxUV[idx * 2 + 1]);
                }
            }
            else if(mapping == "ByPolygon")
            {
                uv.resize(vertices.size() / 3 * 2);
                
                unsigned index = 0;
                for(unsigned k = 0; k < fbxUV.size() / 2; ++k)
                {
                    std::vector<unsigned> polyIndices;
                    for(unsigned l = index; l < fbxUV.size(); ++l)
                        polyIndices.push_back(l);
                    
                    for(unsigned l = 0; l < polyIndices.size(); ++l)
                    {
                        uv[polyIndices[l] * 2] = fbxUV[k * 2];
                        uv[polyIndices[l] * 2 + 1] = fbxUV[k * 2 + 1];
                    }
                }
            }
            else if(mapping == "ByPolygonVertex")
            {
                uv.resize(vertices.size() / 3 * 2);
                
                if(refType == "Direct")
                {
                    for(unsigned k = 0; k < fbxIndices.size(); ++k)
                    {
                        int32_t idx = fbxIndices[k] < 0 ? -fbxIndices[k] - 1 : fbxIndices[k];
                        uv[k * 2] = fbxUV[k * 2];
                        uv[k * 2 + 1] = fbxUV[k * 2 + 1];
                    }
                }
                else if(refType == "IndexToDirect")
                {
                    for(unsigned k = 0; k < fbxIndices.size(); ++k)
                    {
                        int32_t idx = fbxIndices[k] < 0 ? -fbxIndices[k] - 1 : fbxIndices[k];
                        uv[k * 2] = fbxUV[fbxUVIndex[k] * 2];
                        uv[k * 2 + 1] = fbxUV[fbxUVIndex[k] * 2 + 1];
                    }
                }
            }
            
            uvLayers.push_back(uv);
        }

        if(uvLayers.empty())
        {
            uvLayers.push_back(std::vector<float>(vertices.size() / 3 * 2));
        }
    }
};

}}}

#endif
