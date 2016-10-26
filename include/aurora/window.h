#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <windows.h>

namespace Au
{

class Window
{
public:
    static Window Create(const std::string& title = "MyWindow", int width = 800, int height = 600);

    Window();
    ~Window();

    bool Show();
    bool Update();
private:
    HWND hWnd;
    MSG  msg;
};

}

#endif
