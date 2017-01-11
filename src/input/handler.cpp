#include "handler.h"

#include <vector>

namespace Au{
namespace Input{
    
HWND targetWindow;
WNDPROC OldWndProc;

std::vector<RAWINPUTDEVICE> winapiRawInputDevices;

unsigned keyboardKeyState[0xFF]; //255

MouseHandler* mouseHandler = 0;
KeyboardHandler* keyboardHandler = 0;

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
                if (raw->data.keyboard.Flags == RI_KEY_MAKE)
                {
                    keyboardKeyState[raw->data.keyboard.VKey]++;
					if (keyboardHandler && keyboardKeyState[raw->data.keyboard.VKey] == 1)
						keyboardHandler->KeyDown((KEYCODE)raw->data.keyboard.VKey);
                }
                else if (raw->data.keyboard.Flags & RI_KEY_BREAK)
                {
                    keyboardKeyState[raw->data.keyboard.VKey] = 0;
					if (keyboardHandler)
						keyboardHandler->KeyUp((KEYCODE)raw->data.keyboard.VKey);
                }
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

MouseHandler::~MouseHandler()
{
    if(mouseHandler == this)
        mouseHandler = 0;
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
void MouseHandler::KeyUp(KEYCODE key){}
void MouseHandler::KeyDown(KEYCODE key){}
void MouseHandler::Wheel(short value){}

KeyboardHandler::~KeyboardHandler()
{
    if(keyboardHandler == this)
        keyboardHandler = 0;
}

bool KeyboardHandler::Init(Au::Window* window)
{
    if(!ReplaceWindowProc(window))
        return false;
    
    keyboardHandler = this;
    
    RAWINPUTDEVICE device;
    device.usUsagePage = 0x01;
    device.usUsage = 0x06; //RID_KEYBOARD ?
    device.dwFlags = 0;//RIDEV_NOLEGACY don't use
    device.hwndTarget = *window;
    winapiRawInputDevices.push_back(device);
    
    if(!RegisterRawInputDevices(winapiRawInputDevices.data(), winapiRawInputDevices.size(), sizeof(RAWINPUTDEVICE)))
        return false;
    
    return true;
}
    
void KeyboardHandler::KeyUp(KEYCODE key){}
void KeyboardHandler::KeyDown(KEYCODE key){}

}}
