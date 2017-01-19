#include <aurora/window.h>
#include <aurora/gfx.h>
#include <aurora/transform.h>
#include <aurora/input.h>

#include <aurora/media/fbx.h>

#include <fstream>
#include <iostream>

Au::Math::Transform model;
Au::Math::Transform view;
Au::Math::Mat4f projection;
float fov = 1.6f;

class MouseHandler : public Au::Input::MouseHandler
{
public:
    void KeyUp(Au::Input::KEYCODE key)
    {
        std::cout << "Mouse key UP" << std::endl;
    }
    
    void KeyDown(Au::Input::KEYCODE key)
    {
        std::cout << "Mouse key DOWN" << std::endl;
    }
    
    void Move(int x, int y)
    {
        model.Rotate(x * 0.01f, Au::Math::Vec3f(0, 1, 0));
        model.Rotate(y * 0.01f, Au::Math::Vec3f(1, 0, 0));
    }
    
    void Wheel(short value)
    {
        fov -= value * 0.001f;
        projection = Au::Math::Perspective(fov, 4.0f/3.0f, 0.1f, 100);
    }
};

class KeyboardHandler : public Au::Input::KeyboardHandler
{
public:
    void KeyUp(Au::Input::KEYCODE key)
    {
        std::cout << "Keyboard UP" << key <<std::endl;
    }
    
    void KeyDown(Au::Input::KEYCODE key)
    {
        std::cout << "Keyboard DOWN" << key <<std::endl;
    }
};

#pragma pack(push, 1)
struct Vertex
{
    float x, y, z;
    unsigned char r, g, b;
};
#pragma pack(pop)

Au::GFX::Device gfxDevice;

MouseHandler mouseHandler;
KeyboardHandler keyboardHandler;

Au::GFX::Mesh* CreateCubeMesh()
{
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
      0, 5, 1, 5, 0, 4,
      1, 5, 6, 6, 2, 1 };

    Au::GFX::Mesh* mesh = gfxDevice.CreateMesh();
    mesh->Format(Au::Position() << Au::ColorRGB());
    mesh->VertexData(vertices);
    mesh->IndexData(indices);
    
    return mesh;
}

Au::GFX::RenderState* CreateRenderState()
{
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
            color = ColorRGB;
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
    renderState->AttribFormat(Au::Position() << Au::ColorRGB());
    renderState->SetShader(shaderVertex);
    renderState->SetShader(shaderPixel);
    renderState->AddUniform<Au::Math::Mat4f>("MatrixModel");
    renderState->AddUniform<Au::Math::Mat4f>("MatrixView");
    renderState->AddUniform<Au::Math::Mat4f>("MatrixProjection");
    
    std::cout << renderState->StatusString() << std::endl;
    
    return renderState;
}



void Init(Au::Window& window)
{
    gfxDevice.Init(window);
    mouseHandler.Init(&window);
    keyboardHandler.Init(&window);
}

void Cleanup()
{
    gfxDevice.Cleanup();
}

int main()
{
    // FBX Loading WIP =========================
    
    std::ifstream file("miku.fbx", std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);
    if(file.read(buffer.data(), size))
    {
        Au::Media::FBX::Reader fbxReader;
        fbxReader.ReadFile(buffer.data(), buffer.size());
        fbxReader.Print();
    }
    /*
    std::vector<Au::Math::Vec3f> position = fbxReader.GetArray<Au::Math::Vec3f>(Au::Position());
    std::vector<Au::Math::Vec3f> normal = fbxReader.GetData<Au::Normal>();
    std::vector<Au::Math::Vec2f> uv = fbxReader.GetData<Au::UV>();
    */
    // =========================================
    
    Au::Input::LoadDeviceList();
    Au::Window window;
    
    Init(window);
    Au::GFX::Mesh* mesh = CreateCubeMesh();
    Au::GFX::RenderState* renderState = CreateRenderState();   
    
    projection = Au::Math::Perspective(fov, 4.0f/3.0f, 0.1f, 100);
    view.Translate(Au::Math::Vec3f(0.0f, 0.0f, 2.0f));
    
    window.Name("Aurora");
    //window.Resize(640, 480);
    
    Au::GFX::Uniform uniModelMat4f = Au::GFX::GetUniform<Au::Math::Mat4f>("MatrixModel");
    Au::GFX::Uniform uniViewMat4f = Au::GFX::GetUniform<Au::Math::Mat4f>("MatrixView");
    Au::GFX::Uniform uniProjMat4f = Au::GFX::GetUniform<Au::Math::Mat4f>("MatrixProjection");

    if(window.Show())
        while(!window.Destroyed())
        {
            Au::Window::PollMessages();
            
            gfxDevice.Clear();
            gfxDevice << 
                renderState << 
                uniModelMat4f << 
                model.GetTransform() <<
                uniViewMat4f << 
                Au::Math::Inverse(view.GetTransform()) <<
                uniProjMat4f << 
                projection <<
                mesh;
            gfxDevice.SwapBuffers();
        }

    
    Cleanup();
    return 0;
}
