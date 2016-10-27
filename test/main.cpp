#include <aurora/window.h>
#include <iostream>

int main()
{
    Au::Window window;
    
    while(window.Update());
    
    return 0;
}