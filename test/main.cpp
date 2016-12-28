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

    Au::GFX::Device gfxDevice;
    gfxDevice.Init(window);
    
    // Setting up a mesh ============================================

    std::vector<Vertex> vertices =
    { {-0.5f, -0.5f, 0.5f, 255, 100, 0},
      {0.5f, -0.5f, 0.5f, 0, 100, 255},
      {0.5f, 0.5f, 0.5f, 0, 255, 100},
      {-0.5f, 0.5f, 0.5f, 100, 255, 0},
      {-0.5f, -0.5f, -0.5f, 0, 100, 255},
      {0.5f, -0.5f, -0.5f, 0, 255, 100},
      {0.5f, 0.5f, -0.5f, 100, 255, 0},
      {-0.5f, 0.5f, -0.5f, 255, 100, 0} };

    std::vector<unsigned short> indices =
    { 0, 1, 2, 2, 3, 0,
      3, 2, 6, 6, 7, 3,
      7, 6, 5, 5, 4, 7,
      4, 0, 3, 3, 7, 4,
      0, 1, 5, 5, 4, 0,
      1, 5, 6, 6, 2, 1 };

    Au::GFX::Mesh* mesh = gfxDevice.CreateMesh();
    mesh->Format(Au::GFX::Position() << Au::GFX::ColorRGB());
    mesh->VertexData(vertices);
    mesh->IndexData(indices);
    
    // ==============================================================
    
    Au::GFX::Shader* shaderVertex = gfxDevice.CreateShader(Au::GFX::Shader::VERTEX);
    shaderVertex->Source(R"(
        uniform mat4 MatrixModel;
        uniform mat4 MatrixView;
        uniform mat4 MatrixProjection;
        in vec3 Position;
        in vec3 ColorRGB;
        varying vec3 color;
        void main()
        {
            color = cross(ColorRGB, Position);
            gl_Position = MatrixProjection * MatrixView * MatrixModel * vec4(Position, 1.0);
    })");
    std::cout << shaderVertex->StatusString() << std::endl;
    
    Au::GFX::Shader* shaderPixel = gfxDevice.CreateShader(Au::GFX::Shader::PIXEL);
    shaderPixel->Source(R"(
        varying vec3 color;
        void main()
        {
            gl_FragColor = vec4(color, 1.0);
    })");
    std::cout << shaderPixel->StatusString() << std::endl;
    
    Au::GFX::RenderState* renderState = gfxDevice.CreateRenderState();
    renderState->AttribFormat(Au::GFX::Position() << Au::GFX::ColorRGB());
    renderState->SetShader(shaderVertex);
    renderState->SetShader(shaderPixel);
    renderState->AddUniform<Au::Math::Mat4f>("MatrixModel");
    renderState->AddUniform<Au::Math::Mat4f>("MatrixView");
    renderState->AddUniform<Au::Math::Mat4f>("MatrixProjection");
    
    std::cout << renderState->StatusString() << std::endl;
    
    Au::Math::Transform model;
    Au::Math::Transform view;
    Au::Math::Mat4f projection = Au::Math::Perspective(1.6f, 4.0f/3.0f, 0.1f, 100);
    view.Translate(Au::Math::Vec3f(0.0f, 1.0f, 2.0f));
    
    gfxDevice.Bind(mesh);
    gfxDevice.Bind(renderState);
    
    window.Name("Aurora");
    window.Resize(640, 480);

    if(window.Show())
        while(!window.Destroyed())
        {
            Au::Window::PollMessages();
            
            model.Rotate(0.0001f, Au::Math::Vec3f(0, 1, 0));
            
            Au::GFX::Uniform<Au::Math::Mat4f>::Get("MatrixModel") = model.GetTransform();
            Au::GFX::Uniform<Au::Math::Mat4f>::Get("MatrixView") = Au::Math::Inverse(view.GetTransform());
            Au::GFX::Uniform<Au::Math::Mat4f>::Get("MatrixProjection") = projection;
            
            gfxDevice.Clear();
            gfxDevice.Render();
            gfxDevice.SwapBuffers();
        }

    gfxDevice.Cleanup();

    return 0;
}
