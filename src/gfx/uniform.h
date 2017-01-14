#ifndef UNIFORM_H
#define UNIFORM_H

#include <string>
#include <vector>
#include <map>

#include "../math/math.h"

namespace Au{
namespace GFX{

template<typename T>
struct UniformStorage
{
    static std::vector<T> data;
    static std::map<std::string, unsigned int> name_index;
};

template<typename T>
std::vector<T> UniformStorage<T>::data;
template<typename T>
std::map<std::string, unsigned int> UniformStorage<T>::name_index;

class IUniformInterface
{
public:
    virtual ~IUniformInterface() {}
    virtual void Upload(unsigned int location, unsigned int dataIndex) = 0;
};

template<typename T>
class UniformInterface : public IUniformInterface
{
public:
    void Upload(unsigned int location, unsigned int dataIndex);
};

template<>
inline void UniformInterface<float>::Upload(unsigned int location, unsigned int dataIndex) 
{ 
    glUniform1f(location, UniformStorage<float>::data[dataIndex]); 
}
template<>
inline void UniformInterface<Math::Vec2f>::Upload(unsigned int location, unsigned int dataIndex) 
{ 
    glUniform2f(
        location, 
        UniformStorage<Math::Vec2f>::data[dataIndex].x, 
        UniformStorage<Math::Vec2f>::data[dataIndex].y
    ); 
}
template<>
inline void UniformInterface<Math::Vec3f>::Upload(unsigned int location, unsigned int dataIndex) 
{ 
    glUniform3f(
        location, 
        UniformStorage<Math::Vec3f>::data[dataIndex].x, 
        UniformStorage<Math::Vec3f>::data[dataIndex].y, 
        UniformStorage<Math::Vec3f>::data[dataIndex].z
    );
}
template<>
inline void UniformInterface<Math::Vec4f>::Upload(unsigned int location, unsigned int dataIndex) 
{
    glUniform4f(
        location, 
        UniformStorage<Math::Vec4f>::data[dataIndex].x, 
        UniformStorage<Math::Vec4f>::data[dataIndex].y, 
        UniformStorage<Math::Vec4f>::data[dataIndex].z, 
        UniformStorage<Math::Vec4f>::data[dataIndex].w
    );
}
template<>
inline void UniformInterface<int>::Upload(unsigned int location, unsigned int dataIndex) 
{
    glUniform1i(
        location, 
        UniformStorage<int>::data[dataIndex]
    );
}
template<>
inline void UniformInterface<Math::Vec2i>::Upload(unsigned int location, unsigned int dataIndex) 
{
    glUniform2i(
        location,
        UniformStorage<Math::Vec2i>::data[dataIndex].x,
        UniformStorage<Math::Vec2i>::data[dataIndex].y
    );
}
template<>
inline void UniformInterface<Math::Vec3i>::Upload(unsigned int location, unsigned int dataIndex) 
{
    glUniform3i(
        location,
        UniformStorage<Math::Vec3i>::data[dataIndex].x,
        UniformStorage<Math::Vec3i>::data[dataIndex].y,
        UniformStorage<Math::Vec3i>::data[dataIndex].z
    );
}
template<>
inline void UniformInterface<Math::Vec4i>::Upload(unsigned int location, unsigned int dataIndex) 
{
    glUniform4i(
        location,
        UniformStorage<Math::Vec4i>::data[dataIndex].x,
        UniformStorage<Math::Vec4i>::data[dataIndex].y,
        UniformStorage<Math::Vec4i>::data[dataIndex].z,
        UniformStorage<Math::Vec4i>::data[dataIndex].w
    );
}
template<>
inline void UniformInterface<unsigned int>::Upload(unsigned int location, unsigned int dataIndex)
{
    glUniform1ui(
        location, 
        UniformStorage<unsigned int>::data[dataIndex]
    );
}
template<>
inline void UniformInterface<Math::Vec2ui>::Upload(unsigned int location, unsigned int dataIndex) 
{
    glUniform2ui(
        location,
        UniformStorage<Math::Vec2ui>::data[dataIndex].x,
        UniformStorage<Math::Vec2ui>::data[dataIndex].y
    );
}
template<>
inline void UniformInterface<Math::Vec3ui>::Upload(unsigned int location, unsigned int dataIndex) 
{
    glUniform3ui(
        location,
        UniformStorage<Math::Vec3ui>::data[dataIndex].x,
        UniformStorage<Math::Vec3ui>::data[dataIndex].y,
        UniformStorage<Math::Vec3ui>::data[dataIndex].z
    );
}
template<>
inline void UniformInterface<Math::Vec4ui>::Upload(unsigned int location, unsigned int dataIndex) 
{
    glUniform4ui(
        location,
        UniformStorage<Math::Vec4ui>::data[dataIndex].x,
        UniformStorage<Math::Vec4ui>::data[dataIndex].y,
        UniformStorage<Math::Vec4ui>::data[dataIndex].z,
        UniformStorage<Math::Vec4ui>::data[dataIndex].w
    );
}
template<>
inline void UniformInterface<Math::Mat3f>::Upload(unsigned int location, unsigned int dataIndex) 
{
    glUniformMatrix3fv(
        location,
        1,
        GL_FALSE,
        (float*)&(UniformStorage<Math::Mat3f>::data[dataIndex])
    );
}
template<>
inline void UniformInterface<Math::Mat4f>::Upload(unsigned int location, unsigned int dataIndex) 
{
    glUniformMatrix4fv(
        location, 
        1, 
        GL_FALSE, 
        (float*)&(UniformStorage<Math::Mat4f>::data[dataIndex])
    );
}

// ======================================================

template<typename T>
class UniformInterfaceStorage
{
public:
    static UniformInterface<T> impl;
};

template<typename T>
UniformInterface<T> UniformInterfaceStorage<T>::impl;

// ======================================================

class Uniform
{
public:
    Uniform()
    : dataIndex(0), impl(0) {}
    Uniform(unsigned dataIndex, IUniformInterface* impl)
    : dataIndex(dataIndex), impl(impl) {}
    template<typename T>
    Uniform& operator=(const T& value);
    
    unsigned DataIndex() { return dataIndex; }
    
    void Upload(unsigned int location)
    {
        impl->Upload(location, dataIndex);
    }
private:
    unsigned dataIndex;
    IUniformInterface* impl;
};

template<typename T>
Uniform& Uniform::operator=(const T& value)
{
    UniformStorage<T>::data[dataIndex] = value;
    return *this;
}

template<>
inline Uniform& Uniform::operator=(const Uniform& value)
{
    dataIndex = value.dataIndex;
    impl = value.impl;
}

template<typename T>
Uniform GetUniform(const std::string& name)
{
    std::map<std::string, unsigned int>::iterator it;
    it = UniformStorage<T>::name_index.find(name);
    if (it != UniformStorage<T>::name_index.end())
        return Uniform(it->second, &UniformInterfaceStorage<T>::impl);
    else
    {
        unsigned int id = UniformStorage<T>::data.size();
        UniformStorage<T>::name_index.insert(std::make_pair(name, id));
        UniformStorage<T>::data.push_back(T());
        //names.push_back(name);
        return Uniform(id, &UniformInterfaceStorage<T>::impl);
    }
}

}
}

#endif
