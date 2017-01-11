#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "../../include/aurora/window.h"

#include <iostream>

namespace Au{
namespace Input{
    
enum KEYCODE
{
    KEY_NONE, 
    KEY_MOUSE_LEFT, KEY_MOUSE_RIGHT, KEY_MOUSE_MIDDLE, KEY_MOUSE_X1, KEY_MOUSE_X2
};

struct Event
{
    unsigned int deviceId;
    unsigned int keyId;
};

class MouseHandler
{
public:
    bool Init(Au::Window* window);
    
    virtual void Move(int x, int y);
    virtual void KeyUp();
    virtual void KeyDown();
};

inline void LoadDeviceList()
{
    UINT nDevices;
    PRAWINPUTDEVICELIST pRawInputDeviceList;
    
    if(GetRawInputDeviceList(0, &nDevices, sizeof(RAWINPUTDEVICELIST)) != 0)
        return;
    
    std::cout << "Input device count: " << nDevices << std::endl;
    
    pRawInputDeviceList = new RAWINPUTDEVICELIST[nDevices];
    
    if(GetRawInputDeviceList(pRawInputDeviceList, &nDevices, sizeof(RAWINPUTDEVICELIST)) == -1)
    {
        delete pRawInputDeviceList;
        return;
    }
    
    for(UINT i = 0; i < nDevices; ++i)
    {
        UINT dataSize = 0;
        GetRawInputDeviceInfo(pRawInputDeviceList[i].hDevice, 
                              RIDI_DEVICENAME,
                              NULL,
                              &dataSize);
                              
        wchar_t* data = new wchar_t[dataSize];

        GetRawInputDeviceInfo(pRawInputDeviceList[i].hDevice,
                              RIDI_DEVICENAME,
                              (void*)data,
                              &dataSize);
        
        if(dataSize > 0)
        {
        std::wstring deviceName(data, data + dataSize);
        std::wcout << deviceName << std::endl;
        // TODO
        }
        delete data;
    }
    
    delete pRawInputDeviceList;
}
  
}}

#endif
