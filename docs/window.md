## Au::Window

```cpp
#include <aurora/window.h>

Au::Window* window = Au::Window::Create("Title", 800, 600);
window->Show();

while(Au::Window::PollMessages());
// PollMessages() will return false only 
// when all Au::Window instances are closed

Au::Window::Destroy(window);
```
