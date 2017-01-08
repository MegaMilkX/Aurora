#include "handler.h"

#include <set>

namespace Au{
namespace Input{
    
HWND targetWindow;
WNDPROC OldWndProc;
    
std::set<Handler*> gInputHandlers;

LRESULT CALLBACK InputWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_MOUSEMOVE:
        break;
    case WM_INPUT:
        {
            Event event;
            event.deviceId = 0;
            event.keyId = 0;
        }
        break;
    //default:
        //return CallWindowProc(OldWndProc, hWnd, msg, wParam, lParam);
    }
    return 0;
}

Handler::Handler(const Au::Window& window)
{
    gInputHandlers.insert(this);
}

Handler::~Handler()
{
    gInputHandlers.erase(this);
}

void Init(Window* window)
{/*
    HWND hWnd = *window;
    
    OldWndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)InputWndProc);
    if(!OldWndProc)
        return false;
    
    // Initizlize raw input
    
    rid[0].usUsagePage = 0x01;
    rid[0].usUsage = RID_MOUSE;
    rid[0].dwFlags = 0;//RIDEV_NOLEGACY;
    rid[0].hwndTarget = hWnd;
    
    rid[1].usUsagePage = 0x01;
    rid[1].usUsage = RID_KEYBOARD;
    rid[1].dwFlags = 0;
    rid[1].hwndTarget = hWnd;
    
    if(!RegisterRawInputDevices(rid, RID_COUNT, sizeof(RAWINPUTDEVICE)))
        return false;

    targetWindow = hWnd;

	eventHandler = handler;*/
}

}}
