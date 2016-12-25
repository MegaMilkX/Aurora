#include <aurora/window.h>
#include <aurora/gfx.h>
#include <aurora/timeline.h>
#include <aurora/transform.h>

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
    { {-0.5f, -0.5f, 0.5f, 255, 50, 0},
      {0.5f, -0.5f, 0.5f, 0, 50, 255},
      {0.5f, 0.5f, 0.5f, 0, 255, 50},
      {-0.5f, 0.5f, 0.5f, 50, 255, 0},
      {-0.5f, -0.5f, -0.5f, 0, 0, 0},
      {0.5f, -0.5f, -0.5f, 0, 0, 0},
      {0.5f, 0.5f, -0.5f, 0, 0, 0},
      {-0.5f, 0.5f, -0.5f, 0, 0, 0} };

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

    // ==============================================================

    Au::Math::Transform model;
    Au::Math::Transform view;
    Au::Math::Mat4f projection = Au::Math::Perspective(1.6f, 4.0f/3.0f, 0.1f, 100);
    view.Translate(Au::Math::Vec3f(0.0f, 1.0f, 2.0f));
    
    // ==============================================================
    
    Au::GFX::Shader* shader = gfx_device.CreateShader();
    shader->AttribFormat(Au::GFX::Position() << Au::GFX::ColorRGB());
    shader->AddStage(Au::GFX::Shader::PIXEL, R"(
        varying vec3 color;
        void main()
        {
            gl_FragColor = vec4(color, 1.0);
        })");
    shader->AddStage(Au::GFX::Shader::VERTEX, R"(
        uniform mat4 MatrixModel;
        uniform mat4 MatrixView;
        uniform mat4 MatrixProjection;
        in vec3 Position;
        in vec3 ColorRGB;
        varying vec3 color;
        void main()
        {
            color = ColorRGB;
            gl_Position = MatrixProjection * MatrixView * MatrixModel * vec4(Position, 1.0);
        })");
    shader->Compile();
    std::cout << shader->StatusString();
    
    shader->AddUniform(Au::GFX::Uniform<Au::Math::Mat4f>::Get("MatrixModel"));
    shader->AddUniform(Au::GFX::Uniform<Au::Math::Mat4f>::Get("MatrixView"));
    shader->AddUniform(Au::GFX::Uniform<Au::Math::Mat4f>::Get("MatrixProjection"));
    
    window.Name("GAME");
    window.Resize(640, 480);

    if(window.Show())
        while(!window.Destroyed())
        {
            Au::Window::PollMessages();
            
            Au::GFX::Uniform<Au::Math::Mat4f>::Get("MatrixModel") = model.GetTransform();
            Au::GFX::Uniform<Au::Math::Mat4f>::Get("MatrixView") = Au::Math::Inverse(view.GetTransform());
            Au::GFX::Uniform<Au::Math::Mat4f>::Get("MatrixProjection") = projection;
            
            gfx_device.Bind(mesh);
            gfx_device.Bind(shader);
            
            model.Rotate(0.0001f, Au::Math::Vec3f(0, 1, 0));
            shader->SetUniform("MatrixModel", model.GetTransform());
            
            gfx_device.Clear();
            gfx_device.Render();
            gfx_device.SwapBuffers();
        }

    gfx_device.Destroy(mesh);
    gfx_device.Destroy(shader);
    gfx_device.Cleanup();

    return 0;
}
