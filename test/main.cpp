#include <aurora/window.h>
#include <aurora/gfx.h>
#include <aurora/sfx.h>
#include <aurora/input.h>
#include <aurora/gamestate.h>

class InputHandler : public Au::Input::Handler
{
public:
    void OnKeyDown(const Au::Input::Event& e)
    {
        
    }
    
    void OnKeyUp(const Au::Input::Event& e)
    {
        Au::GameState::OnInput(e);
    }
};

int main()
{
    Au::Window window;
    
    InputHandler input_handler(window);
    
    if(window.Show())
        while(window.Update());
    return 0;
}
