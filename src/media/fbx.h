#ifndef AURORA_FBX_H
#define AURORA_FBX_H

#include <vector>
#include <string>

namespace Au{
namespace Media{

class MeshObject
{
public:  
};

std::vector<MeshObject> LoadFromFBX(const std::string& filename);

}
}

#endif
