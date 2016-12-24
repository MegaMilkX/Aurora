#ifndef GLBUFFER_H
#define GLBUFFER_H

#include "glextutil.h"

namespace Au{
namespace GFX{

class GLBuffer
{
public:
    GLBuffer() : buffer(0), target(0), usage(0) {}
    static GLBuffer Create(unsigned int target, unsigned int usage_hint);
	void Destroy();
    bool Data(void* data, size_t sz);
    bool Valid();
    void Bind();
private:
    
    unsigned int buffer;
    unsigned int target;
    unsigned int usage;
};

}
}

#endif