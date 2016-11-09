#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include #include "../../include/aurora/window.h"

namespace Au{
namespace Input{

struct Event
{
    
};

class Handler
{
public:
    Handler(const Au::Window& window);
    virtual Handler();
    
    virtual void OnInput(const Event& e);
    virtual void OnDown(const Event& e);
    virtual void OnUp(const Event& e);
    virtual void OnMouseMove(const Event& e);
};
  
}}

#endif
