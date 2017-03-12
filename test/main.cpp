#include <aurora/window.h>
#include <aurora/gfx.h>
#include <aurora/gfx/armature.h>
#include <aurora/transform.h>
#include <aurora/input.h>

#include <aurora/media/fbx.h>

#include <fstream>
#include <iostream>

Au::Math::Transform model;
Au::Math::Transform view;
Au::Math::Mat4f projection;
float fov = 1.6f;
float zfar = 100.0f;
bool mouseDown = false;

class MouseHandler : public Au::Input::MouseHandler
{
public:
    void KeyUp(Au::Input::KEYCODE key)
    {
        mouseDown = false;
    }
    
    void KeyDown(Au::Input::KEYCODE key)
    {
        mouseDown = true;
    }
    
    void Move(int x, int y)
    {        
        if(!mouseDown)
            return;
        model.Rotate(x * 0.01f, Au::Math::Vec3f(0, 1, 0));
        //model.Rotate(y * 0.01f, model.GetTransform() * Au::Math::Vec3f(1, 0, 0));
    }
    
    void Wheel(short value)
    {
        fov -= value * 0.001f;
        projection = Au::Math::Perspective(fov, 4.0f/3.0f, 0.1f, zfar);
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

Au::GFX::Mesh* LoadMesh(const std::string& path)
{
    Au::GFX::Mesh* mesh = gfxDevice.CreateMesh();
    mesh->Format(Au::Position() << Au::Normal() << Au::ColorRGB());
    
    // FBX Loading WIP =========================
    
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);
    if(file.read(buffer.data(), size))
    {
        Au::Media::FBX::Reader fbxReader;
        fbxReader.ReadFile(buffer.data(), buffer.size());
        fbxReader.ConvertCoordSys(Au::Media::FBX::OPENGL);
        //fbxReader.Print();
        
        std::vector<float> vertices = fbxReader.GetVertices(0);
        mesh->VertexAttrib<Au::Position>(vertices);
        
        std::vector<float> normals = fbxReader.GetNormals(0, 0);
        mesh->VertexAttrib<Au::Normal>(normals);
        
        std::vector<unsigned short> indices = fbxReader.GetIndices<unsigned short>(0);
        mesh->IndexData(indices);
        
        //fbxReader.GetWeights<unsigned char>(0);
        //fbxReader.GetBoneIndices<unsigned char>(0);        
    }
    // =========================================
    
    return mesh;
}

Au::GFX::Armature LoadArmature(const std::string& path)
{
    Au::GFX::Armature armature;
    
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);
    if(file.read(buffer.data(), size))
    {
        Au::Media::FBX::Reader fbxReader;
        fbxReader.ReadFile(buffer.data(), buffer.size());
        
        std::vector<Au::Media::FBX::Bone> bones = fbxReader.GetBones();
        
        for(unsigned i = 0; i < bones.size(); ++i)
        {    
            armature.GetBone(bones[i].uid)->parentUID = bones[i].parentUID;
            armature.GetBone(bones[i].uid)->bindTransform = bones[i].bindTransform;
            armature.GetBone(bones[i].uid)->name = bones[i].name;
        }
    }
    
    return armature;
}

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

Au::GFX::Mesh* Create3DCrossMesh()
{
    std::vector<float> vertices =
    { -1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, 1.0f };

    std::vector<unsigned short> indices =
    { 0, 1, 2, 3, 4, 5 };

    Au::GFX::Mesh* mesh = gfxDevice.CreateMesh();
    mesh->PrimitiveType(Au::GFX::Mesh::LINE);
    mesh->Format(Au::Position());
    mesh->VertexData(vertices.data(), vertices.size()/3);
    mesh->IndexData(indices);
    
    return mesh;
}

Au::GFX::RenderState* CreateRenderState()
{
    Au::GFX::Shader* shaderVertex = gfxDevice.CreateShader(Au::GFX::Shader::VERTEX);
    shaderVertex->Source(R"(#version 120
        uniform mat4 MatrixModel;
        uniform mat4 MatrixView;
        uniform mat4 MatrixProjection;
        in vec3 Position;
        in vec3 Normal;
        in vec3 ColorRGB;
        varying vec3 color;
        varying vec3 normal;
        
        varying vec3 fragPos;
        
        uniform mat4 Bones[32];
        in vec4 BoneWeights;
        in ivec4 BoneIndices;
        
        void main()
        {
            vec3 skinnedPos = (BoneWeights[0] * (Bones[BoneIndices[0]] * vec4(Position, 1.0)) + 
                              BoneWeights[1] * (Bones[BoneIndices[1]] * vec4(Position, 1.0)) + 
                              BoneWeights[2] * (Bones[BoneIndices[2]] * vec4(Position, 1.0)) + 
                              BoneWeights[3] * (Bones[BoneIndices[3]] * vec4(Position, 1.0))
                              ).xyz;
            vec3 skinnedNormal = BoneWeights[0] * (mat3(Bones[BoneIndices[0]]) * Normal) +
                                 BoneWeights[1] * (mat3(Bones[BoneIndices[1]]) * Normal) +
                                 BoneWeights[2] * (mat3(Bones[BoneIndices[2]]) * Normal) +
                                 BoneWeights[3] * (mat3(Bones[BoneIndices[3]]) * Normal);
            
            skinnedNormal = normalize(skinnedNormal);
            
            fragPos = vec3(MatrixModel * vec4(skinnedPos, 1.0));
            color = ColorRGB;
            normal = (MatrixModel * vec4(skinnedNormal, 1.0)).xyz;
            gl_Position = MatrixProjection * MatrixView * MatrixModel * vec4(skinnedPos, 1.0);
    })");
    std::cout << shaderVertex->StatusString() << std::endl;
    
    Au::GFX::Shader* shaderPixel = gfxDevice.CreateShader(Au::GFX::Shader::PIXEL);
    shaderPixel->Source(R"(#version 120
        varying vec3 color;
        varying vec3 normal;
        varying vec3 fragPos;
        
        uniform vec3 LightOmniPos[3];
        uniform vec3 LightOmniRGB[3];
        
        void main()
        {
            vec3 ambient_color = vec3(0.2, 0.2, 0.2);            
            vec3 result = ambient_color;
            
            for(int i = 0; i < 3; i++)
            {
                vec3 lightDir = normalize(LightOmniPos[i] - fragPos);
                float diff = max(dot(normal, lightDir), 0.0);
                vec3 diffuse = diff * LightOmniRGB[i];
                result += diffuse;
            }
            
            gl_FragColor = vec4(result, 1.0);
    })");
    std::cout << shaderPixel->StatusString() << std::endl;
    
    Au::GFX::RenderState* renderState = gfxDevice.CreateRenderState();
    renderState->AttribFormat(Au::Position() << Au::Normal() << Au::ColorRGB());
    renderState->SetShader(shaderVertex);
    renderState->SetShader(shaderPixel);
    renderState->AddUniform<Au::Math::Mat4f>("MatrixModel");
    renderState->AddUniform<Au::Math::Mat4f>("MatrixView");
    renderState->AddUniform<Au::Math::Mat4f>("MatrixProjection");
    renderState->AddUniform<Au::Math::Vec3f>("LightOmniPos", 3);
    renderState->AddUniform<Au::Math::Vec3f>("LightOmniRGB", 3);
    renderState->AddUniform<Au::Math::Mat4f>("Bones", 32);
    
    std::cout << renderState->StatusString() << std::endl;
    
    return renderState;
}

Au::GFX::RenderState* CreateLinesRenderState()
{
    Au::GFX::Shader* shaderVertex = gfxDevice.CreateShader(Au::GFX::Shader::VERTEX);
    shaderVertex->Source(R"(
        uniform mat4 MatrixModel;
        uniform mat4 MatrixView;
        uniform mat4 MatrixProjection;
        in vec3 Position;
        void main()
        {
            gl_Position = MatrixProjection * MatrixView * MatrixModel * vec4(Position, 1.0);
        })");
    std::cout << shaderVertex->StatusString() << std::endl;
    
    Au::GFX::Shader* shaderPixel = gfxDevice.CreateShader(Au::GFX::Shader::PIXEL);
    shaderPixel->Source(R"(
        varying vec3 color;
        varying vec3 normal;
        void main()
        {            
            gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
        })");
    std::cout << shaderPixel->StatusString() << std::endl;
    
    Au::GFX::RenderState* renderState = gfxDevice.CreateRenderState();
    renderState->AttribFormat(Au::Position());
    renderState->SetShader(shaderVertex);
    renderState->SetShader(shaderPixel);
    renderState->AddUniform<Au::Math::Mat4f>("MatrixModel");
    renderState->AddUniform<Au::Math::Mat4f>("MatrixView");
    renderState->AddUniform<Au::Math::Mat4f>("MatrixProjection");
    renderState->DepthTest(false);
    
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

class Game
{
public:
};

//class GameState01 : public Au::StateStack<Game>::State
//{
//public:
//    
//};

int main()
{
    //Game game;
    //StateStack<Game> stateStack(&game);
    //stateStack.Push<GameState01>();
    //stateStack.Tick();

    //Au::Input::LoadDeviceList();
    Au::Window window;
    
    Init(window);
    Au::GFX::Mesh* mesh = LoadMesh("miku.fbx");
    Au::GFX::RenderState* renderState = CreateRenderState();
    Au::GFX::Armature armature = LoadArmature("skin.fbx");
    Au::GFX::Mesh* cross = Create3DCrossMesh();
    Au::GFX::RenderState* renderStateLines = CreateLinesRenderState();
    
    projection = Au::Math::Perspective(fov, 4.0f/3.0f, 0.1f, zfar);
    view.Translate(Au::Math::Vec3f(0.0f, 1.5f, 7.0f));
    
    window.Name("Aurora");
    //window.Resize(640, 480);
    
    Au::GFX::Uniform uniModelMat4f = Au::GFX::GetUniform<Au::Math::Mat4f>("MatrixModel");
    Au::GFX::Uniform uniViewMat4f = Au::GFX::GetUniform<Au::Math::Mat4f>("MatrixView");
    Au::GFX::Uniform uniProjMat4f = Au::GFX::GetUniform<Au::Math::Mat4f>("MatrixProjection");
    
    Au::GFX::Uniform uniLightOmniPos = Au::GFX::GetUniform<Au::Math::Vec3f>("LightOmniPos", 3);
    Au::GFX::Uniform uniLightOmniRGB = Au::GFX::GetUniform<Au::Math::Vec3f>("LightOmniRGB", 3);
    uniLightOmniPos.Set(Au::Math::Vec3f(0.0f, 1.5f, 4.0f), 0);
    uniLightOmniRGB.Set(Au::Math::Vec3f(0.8f, 0.6f, 0.2f), 0);
    uniLightOmniPos.Set(Au::Math::Vec3f(4.0f, 0.0f, 0.0f), 1);
    uniLightOmniRGB.Set(Au::Math::Vec3f(0.6f, 0.8f, 0.2f), 1);
    uniLightOmniPos.Set(Au::Math::Vec3f(-4.0f, 0.0f, 0.0f), 2);
    uniLightOmniRGB.Set(Au::Math::Vec3f(0.2f, 0.6f, 0.8f), 2);
    
    Au::GFX::Uniform uniBones = Au::GFX::GetUniform<Au::Math::Mat4f>("Bones", 32);
    
    if(window.Show())
        while(!window.Destroyed())
        {
            Au::Window::PollMessages();
            
            gfxDevice.Clear();
            gfxDevice.Bind(renderState);
            gfxDevice.Set(uniModelMat4f, model.GetTransform());
            gfxDevice.Set(uniViewMat4f, Au::Math::Inverse(view.GetTransform()));
            gfxDevice.Set(uniProjMat4f, projection);
            //armature.Upload(gfxDevice, uniBones);
            gfxDevice.Bind(mesh);
            gfxDevice.Render();

            gfxDevice.Bind(renderStateLines);
            Au::GFX::Bone* bones = armature.GetBones();
            unsigned boneCount = armature.BoneCount();
            for(unsigned i = 0; i < boneCount; ++i)
            {
                gfxDevice.Set(uniModelMat4f, model.GetTransform() * bones[i].bindTransform);
                gfxDevice.Set(uniViewMat4f, Au::Math::Inverse(view.GetTransform()));
                gfxDevice.Set(uniProjMat4f, projection);
                gfxDevice.Bind(cross);
                gfxDevice.Render();
            }
            gfxDevice.SwapBuffers();
        }

    
    Cleanup();
    return 0;
}
