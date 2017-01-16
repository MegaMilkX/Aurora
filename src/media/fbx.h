#ifndef FBX_H
#define FBX_H

#include <vector>

namespace Au{
namespace Media{
namespace FBX{

bool Read(const char* data, unsigned size);

class Reader
{
public:
    bool Read(const char* data, unsigned size);
    template<typename T>
    std::vector<T> GetArray(unsigned int attribute);
};

template<typename T>
std::vector<T> Reader::GetArray(unsigned int attribute)
{
    
}

}
}
}

#endif
