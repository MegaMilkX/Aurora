#include "handler.h"

#include <vector>

namespace Au{
namespace Input{
    
HWND targetWindow;
WNDPROC OldWndProc;

std::vector<RAWINPUTDEVICE> winapiRawInputDevices;

MouseHandler* mouseHandler = 0;

LRESULT CALLBACK InputWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_MOUSEMOVE:
        break;
    case WM_INPUT:
        {
            UINT sz = 0;
            GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &sz, sizeof(RAWINPUTHEADER));
            
            std::vector<BYTE> lpb;
            lpb.resize(sz);
            
            if(GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb.data(), &sz, sizeof(RAWINPUTHEADER)) != sz)
                return 0;
            
            RAWINPUT* raw = (RAWINPUT*)lpb.data();

            //raw->header.hDevice; // Unique device handle
         
            if(raw->header.dwType == RIM_TYPEKEYBOARD)
            {
                
            }
            else if(raw->header.dwType == RIM_TYPEMOUSE)
            {
                if(mouseHandler)
                {
                    int xPosRel = raw->data.mouse.lLastX;
                    int yPosRel = raw->data.mouse.lLastY;
                    if(xPosRel | yPosRel)
                        mouseHandler->Move(xPosRel, yPosRel);
                    
                    //raw->data.mouse.usButtonFlags;
                }
            }
        }
        break;
    default:
        return CallWindowProc(OldWndProc, hWnd, msg, wParam, lParam);
    }
    return 0;
}

bool ReplaceWindowProc(Window* window)
{
    if(!OldWndProc)
    {
        OldWndProc = (WNDPROC)SetWindowLongPtr(*window, GWLP_WNDPROC, (LONG_PTR)InputWndProc);
        if(!OldWndProc)
            return false;
    }
    return true;
}

bool MouseHandler::Init(Au::Window* window)
{
    if(!ReplaceWindowProc(window))
        return false;
    
    mouseHandler = this;
    RAWINPUTDEVICE device;
    device.usUsagePage = 0x01;
    device.usUsage = 0x02; //RID_MOUSE ?
    device.dwFlags = 0;//RIDEV_NOLEGACY don't use
    device.hwndTarget = *window;
    winapiRawInputDevices.push_back(device);
    
    if(!RegisterRawInputDevices(winapiRawInputDevices.data(), winapiRawInputDevices.size(), sizeof(RAWINPUTDEVICE)))
        return false;
    
    return true;
}
    
void MouseHandler::Move(int x, int y){}
void MouseHandler::KeyUp(){}
void MouseHandler::KeyDown(){}

}}
