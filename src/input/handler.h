#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "../../include/aurora/window.h"

#include <iostream>

namespace Au{
namespace Input{

struct Event
{
    unsigned int deviceId;
    unsigned int keyId;
};

class Handler
{
public:
    Handler(const Au::Window& window);
    virtual ~Handler();
    
    virtual void OnInput(const Event& e);
    virtual void OnDown(const Event& e);
    virtual void OnUp(const Event& e);
    virtual void OnMouseMove(const Event& e);
};

void LoadDeviceList()
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
