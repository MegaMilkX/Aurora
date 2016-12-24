#ifndef TYPEINFO_H
#define TYPEINFO_H

namespace Au{

typedef int typeindex;

template<typename T>
struct TypeInfo
{
    static typeindex Index()
    {
        static typeindex id = _NewId();
        return id;
    }
private:
};

inline typeindex _NewId()
{
    static typeindex id;
    return ++id;
}

}

#endif