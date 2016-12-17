#include "window.h"

#include <codecvt>
#include <sstream>
#include <map>
#include <iostream>

namespace Au
{

MSG Window::msg = { 0 };
int Window::windowClassId = 0;
int gTotalActiveWindows = 0;

std::map<HWND, bool*> flags_destroyed;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
    switch(msg)
    {
    case WM_CLOSE:
        gTotalActiveWindows--;
        DestroyWindow(hWnd);
        *flags_destroyed[hWnd] = true;
        break;
    case WM_DESTROY:
        if(gTotalActiveWindows == 0)
            PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    
    return result;
}

Window::Window(const std::string& title, int width, int height)
 : hWnd(NULL)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring w_name = converter.from_bytes(title.c_str());

    HINSTANCE hInstance = GetModuleHandle(0);
    
    // Create a WINAPI class name
    std::wostringstream os;
    os << windowClassId;
    std::wstring w_class_name = os.str();
    windowClassId++;
    //

    WNDCLASSEX wc;
    
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = w_class_name.c_str();
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    
    if(!RegisterClassEx(&wc))
    {
        return;
    }

    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = width;
    rect.bottom = height;

	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, 0);
    
    hWnd = CreateWindowExW(
        0,
        w_class_name.c_str(),
        w_name.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
        NULL, NULL, hInstance, NULL
    );
    
    gTotalActiveWindows++;
    
    destroyed = new bool(false);
    flags_destroyed[hWnd] = destroyed;

    return;
}

Window::~Window()
{
    CloseWindow(hWnd);
    flags_destroyed.erase(hWnd);
    delete destroyed;
}

bool Window::Show()
{
    if(hWnd == NULL)
        return false;
    
    ShowWindow(hWnd, 1);
    UpdateWindow(hWnd);
    
    return true;
}

void Window::Name(const std::string& name)
{
    SetWindowTextA(hWnd, name.c_str());
}

void Window::Resize(unsigned width, unsigned height)
{
    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = width;
    rect.bottom = height;

	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, 0);
    
    SetWindowPos(hWnd, 0, 0, 0, rect.right, rect.bottom, SWP_NOMOVE);
}

bool Window::PollMessages()
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

bool Window::Destroyed()
{
    return *destroyed;
}

bool Window::operator<(const Window& other)
{
    return hWnd < other.hWnd;
}

}
