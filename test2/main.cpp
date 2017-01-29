
#include <aurora/gfx/gfx_macro.h>

AU_GFX_DEFINE_RENDER_SYSTEM(
    (
        GFX_STATIC
    ),
    (
        GFX_LINES
    )
)

int main()
{
    Au::Window window;
    AuGFXInit(&window);
    
    if(window.Show())
        while(!window.Destroyed())
        {
            Au::Window::PollMessages();
            
            AuGFXRender();
        }
        
    AuGFXCleanup();
}