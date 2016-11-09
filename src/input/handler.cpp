#include "handler.h"

#include <set>

namespace Au{
namespace Input{
    
std::set<Handler*> gInputHandlers;

LRESULT CALLBACK InputWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_MOUSEMOVE:
        break;
    case WM_INPUT:
        {
        }
        break;
    default:
        return CallWindowProc(OldWndProc, hWnd, msg, wParam, lParam);
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

}}
