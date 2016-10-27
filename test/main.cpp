#include <aurora/window.h>

int main()
{
    Au::Window window;
    if(window.Show())
        while(window.Update());
    return 0;
}