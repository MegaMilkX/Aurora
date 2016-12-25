#ifndef UNIFORM_H
#define UNIFORM_H

#include <string>
#include <vector>
#include <map>

#include "../math.h"

namespace Au{
namespace GFX{

class IUniform
{
public:
    IUniform() : dataIndex(0) {}
    virtual ~IUniform() {};
    virtual void Upload(unsigned int location) = 0;
    unsigned int DataIndex() { return dataIndex; }
    virtual std::string Name() const = 0;
    virtual IUniform* Copy() const = 0;
protected:
    unsigned int dataIndex;
};

template<typename TYPE>
class Uniform : public IUniform
{
public:
    Uniform() {}
    Uniform(unsigned int idx) 
    {
        dataIndex = idx;
    }
    static Uniform Get(const std::string& name)
    {
        std::map<std::string, unsigned int>::iterator it;
        it = name_index.find(name);
        if (it != name_index.end())
            return Uniform<TYPE>(it->second);
        else
        {
            unsigned int id = data.size();
            name_index.insert(std::make_pair(name, id));
            data.push_back(TYPE());
            names.push_back(name);
            Uniform<TYPE> glob(id);
            return glob;
        }
    }

    void operator=(const TYPE& value)
    {
        Uniform<TYPE>::data[dataIndex] = value;
    }

    operator TYPE()
    {
        return Uniform<TYPE>::data[dataIndex];
    }

    void Upload(unsigned int location) {}
    
    std::string Name() const { return names[dataIndex]; }

    IUniform* Copy() const
    {
        return new Uniform<TYPE>(dataIndex);
    }
private:
    static std::map<std::string, unsigned int> name_index;
    static std::vector<TYPE> data;
    static std::vector<std::string> names;
};

template<typename TYPE>
std::map<std::string, unsigned int> Uniform<TYPE>::name_index;
template<typename TYPE>
std::vector<TYPE> Uniform<TYPE>::data;
template<typename TYPE>
std::vector<std::string> Uniform<TYPE>::names;

template<>
void Uniform<float>::Upload(unsigned int location) 
{ glUniform1f(location, data[dataIndex]); }
template<>
void Uniform<Math::Vec2f>::Upload(unsigned int location) 
{ glUniform2f(location, data[dataIndex].x, data[dataIndex].y); }
template<>
void Uniform<Math::Vec3f>::Upload(unsigned int location) 
{ glUniform3f(location, data[dataIndex].x, data[dataIndex].y, data[dataIndex].z); }
template<>
void Uniform<Math::Vec4f>::Upload(unsigned int location) 
{ glUniform4f(location, data[dataIndex].x, data[dataIndex].y, data[dataIndex].z, data[dataIndex].w); }
template<>
void Uniform<int>::Upload(unsigned int location) 
{ glUniform1i(location, data[dataIndex]); }
template<>
void Uniform<Math::Vec2i>::Upload(unsigned int location) 
{ glUniform2i(location, data[dataIndex].x, data[dataIndex].y); }
template<>
void Uniform<Math::Vec3i>::Upload(unsigned int location) 
{ glUniform3i(location, data[dataIndex].x, data[dataIndex].y, data[dataIndex].z); }
template<>
void Uniform<Math::Vec4i>::Upload(unsigned int location) 
{ glUniform4i(location, data[dataIndex].x, data[dataIndex].y, data[dataIndex].z, data[dataIndex].w); }
template<>
void Uniform<unsigned int>::Upload(unsigned int location) { glUniform1ui(location, data[dataIndex]); }
template<>
void Uniform<Math::Vec2ui>::Upload(unsigned int location) 
{ glUniform2ui(location, data[dataIndex].x, data[dataIndex].y); }
template<>
void Uniform<Math::Vec3ui>::Upload(unsigned int location) 
{ glUniform3ui(location, data[dataIndex].x, data[dataIndex].y, data[dataIndex].z); }
template<>
void Uniform<Math::Vec4ui>::Upload(unsigned int location) 
{ glUniform4ui(location, data[dataIndex].x, data[dataIndex].y, data[dataIndex].z, data[dataIndex].w); }
template<>
void Uniform<Math::Mat3f>::Upload(unsigned int location) 
{ glUniformMatrix3fv(location, 1, GL_FALSE, (float*)&(data[dataIndex])); }
template<>
void Uniform<Math::Mat4f>::Upload(unsigned int location) 
{ glUniformMatrix4fv(location, 1, GL_FALSE, (float*)&(data[dataIndex])); }


}
}

#endif
