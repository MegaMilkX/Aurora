#include <aurora/window.h>
//#include <aurora/gfx.h>
//#include <aurora/sfx.h>
//#include <aurora/input.h>
//#include <aurora/gamestate.h>
/*
class InputHandler : public Au::Input::Handler
{
public:
    InputHandler(const Au::Window& window)
    : Au::Input::Handler(window) 
    {}
    void OnKeyDown(const Au::Input::Event& e)
    {
        
    }
    
    void OnKeyUp(const Au::Input::Event& e)
    {
        
    }
};
*/
int main()
{
    Au::Window window;
    Au::Window w2;
    
    //InputHandler input_handler(window);
    
    window.Name("GAME");
    
    if(window.Show() && w2.Show())
        while(Au::Window::PollMessages());
    
    return 0;
}
