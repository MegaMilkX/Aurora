#include "../include/aurora/window.h"

#include <codecvt>

namespace Au
{

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

Window::Window()
{
    
}

Window::~Window()
{
}

bool Window::Show()
{

}

bool Window::Update()
{
    while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if(msg.message == WM_QUIT)
            return false;
    }
    return true;
}

}
