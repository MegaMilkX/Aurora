#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

namespace Au{
namespace Input{

struct Event
{
    
};

class Handler
{
public:
    Handler();
    virtual Handler();
    
    virtual void OnInput(const Event& e);
    virtual void OnDown(const Event& e);
    virtual void OnUp(const Event& e);
    virtual void OnMouseMove(const Event& e);
};
  
}}

#endif
