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
    bool Update();
private:
    HWND hWnd;
    MSG  msg;
};

}

#endif
