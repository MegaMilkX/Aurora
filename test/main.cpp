#include <aurora/window.h>
#include <aurora/gfx.h>
#include <aurora/timeline.h>

#include <iostream>

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

class InterpolatorStr
{
public:
    std::string operator()(const Au::Keyframe<std::string> first, const Au::Keyframe<std::string> second, unsigned int time)
    {
        return first.value;
    }
};

int main()
{
    Au::Window window;

    //InputHandler input_handler(window);
    
    Au::GFX::Device gfx_device;
    gfx_device.Init(window);
    
    Au::GFX::Mesh mesh = gfx_device.CreateMesh();
    mesh.Attrib<Au::GFX::Position,
                Au::GFX::Normal,
                Au::GFX::ColorRGB>
                ();
    
    Au::GFX::Shader shader;
    shader.AddStage(Au::GFX::Shader::PIXEL, R"(
        void main()
        {
            gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
        })");
    shader.AddStage(Au::GFX::Shader::VERTEX, R"(
        void main()
        {
            gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
        })");
    shader.Compile();
    std::cout << shader.StatusString();

    window.Name("GAME");
    window.Resize(640, 480);

    if(window.Show())
        while(!window.Destroyed())
        {
            Au::Window::PollMessages();
            gfx_device.Clear();
            gfx_device.Render();
            gfx_device.SwapBuffers();
        }
        
    gfx_device.Cleanup();

    return 0;
}
