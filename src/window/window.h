#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <windows.h>

namespace Au
{

class Window
{
public:
    Window(const std::string& title = "MyWindow", int width = 800, int height = 600);
    ~Window();
    bool Show();
    
    void Name(const std::string& name);
    void Resize(unsigned width, unsigned height);
    
    bool Destroyed();
    
    bool operator<(const Window& other);
    
    static bool PollMessages();
private:
    Window(const Window& other){}
    Window& operator=(const Window& other){}

    HWND hWnd;
    static MSG msg;
    
    bool* destroyed;
    
    static int windowClassId;
};

}

#endif
