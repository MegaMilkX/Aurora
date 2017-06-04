#ifndef AU_FBXMESH_H
#define AU_FBXMESH_H

#include <vector>
#include <string>

#include "fbxsettings.h"

namespace Au{
namespace Media{
namespace FBX{
   

class SkinDeformer
{
public:
private:
    std::string name;
    std::vector<int64_t> indices;
    std::vector<float> weights;
};

class Skin
{
public:
    int DeformerCount() { return deformers.size(); }
    SkinDeformer* GetDeformer(unsigned index) { return &deformers[index]; }
private:
    std::vector<SkinDeformer> deformers;
};

class Mesh
{
public:
    Mesh() {}
    Mesh(int64_t uid, Axis right, Axis up, Axis front)
    : uid(uid), right(right), up(up), front(front) {}
    
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
    template<typename T>
    std::vector<T> GetIndices()
    {
        std::vector<T> result;
        for(unsigned i = 0; i < indices.size(); ++i)
            result.push_back((T)indices[i]);
        return result;
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
};

}}}

#endif
