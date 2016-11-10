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
    
    static bool PollMessages();
    
    bool operator<(const Window& other);
private:
    HWND hWnd;
    static MSG msg;
    
    static int windowClassId;
};

}

#endif
