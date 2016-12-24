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

#pragma pack(push, 1)
struct Vertex
{
    float x, y, z;
    unsigned char r, g, b;
};
#pragma pack(pop)

int main()
{
    Au::Window window;

    //InputHandler input_handler(window);

    Au::GFX::Device gfx_device;
    gfx_device.Init(window);

    // Setting up a mesh ============================================

    std::vector<Vertex> vertices =
    { {-0.5f, -0.5f, 0.5f, 255, 50, 255},
      {0.5f, -0.5f, 0.5f, 255, 50, 255},
      {0.5f, 0.5f, 0.5f, 255, 50, 255},
      {-0.5f, 0.5f, 0.5f, 255, 50, 255},
      {-0.5f, -0.5f, -0.5f, 255, 50, 255},
      {0.5f, -0.5f, -0.5f, 255, 50, 255},
      {0.5f, 0.5f, -0.5f, 255, 50, 255},
      {-0.5f, 0.5f, -0.5f, 255, 50, 255} };

    std::vector<unsigned short> indices =
    { 0, 1, 2, 2, 3, 0,
      3, 2, 6, 6, 7, 3,
      7, 6, 5, 5, 4, 7,
      4, 0, 3, 3, 7, 4,
      0, 1, 5, 5, 4, 0,
      1, 5, 6, 6, 2, 1 };

    Au::GFX::Mesh* mesh = gfx_device.CreateMesh();
    mesh->Format(Au::GFX::Position() << Au::GFX::ColorRGB());
    mesh->PrintFormat();
    mesh->VertexData(vertices);
    mesh->IndexData(indices);

    gfx_device.Bind(mesh);

    // ==============================================================

    Au::GFX::Shader* shader = gfx_device.CreateShader();
    shader->AddStage(Au::GFX::Shader::PIXEL, R"(
        void main()
        {
            gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
        })");
    shader->AddStage(Au::GFX::Shader::VERTEX, R"(
        void main()
        {
            gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
        })");
    shader->Compile();
    std::cout << shader->StatusString();

    //gfx_device.Bind(shader);
    
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

    gfx_device.Destroy(mesh);
    gfx_device.Destroy(shader);
    gfx_device.Cleanup();

    return 0;
}
