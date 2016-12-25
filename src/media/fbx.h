#ifndef AURORA_FBX_H
#define AURORA_FBX_H

#include <vector>
#include <string>

#include "../utils/file.h"

namespace Au{
namespace Media{

class MeshObject
{
public:  
};

std::vector<MeshObject> LoadFromFBX(const std::string& filename);
std::vector<MeshObject> LoadFromFBX(const File& file);
std::vector<MeshObject> LoadFromFBX(const void* data, size_t sz);

}
}

#endif
