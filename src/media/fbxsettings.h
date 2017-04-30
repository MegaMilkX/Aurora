#ifndef AU_FBX_SETTINGS_H
#define AU_FBX_SETTINGS_H

#include <iostream>
#include <string>

namespace Au{
namespace Media{
namespace FBX{
    
enum CoordSystem
{
    ORIGINAL,
    OPENGL,
    DIRECT3D
};

enum Axis
{
    AXIS_UNKNOWN = 3,
    AXIS_X = 0, AXIS_Y = 1, AXIS_Z = 2,
    AXIS_MX = -1, AXIS_MY = -2, AXIS_MZ = -3
};

inline bool IsSameAxisSign(Axis a, Axis b)
{
    return (((int)a < 0) == ((int)b < 0));
}

inline std::string AxisToStr(Axis axis)
{
    if(axis == AXIS_X)
        return "x";
    else if(axis == AXIS_Y)
        return "y";
    else if(axis == AXIS_Z)
        return "z";
    else if(axis == AXIS_MX)
        return "-x";
    else if(axis == AXIS_MY)
        return "-y";
    else if(axis == AXIS_MZ)
        return "-z";
    else
        return "unknown";
}

struct AxisSetup
{
    Axis right;
    Axis up;
    Axis front;
    
    void Print()
    {
        std::cout << "right: " << AxisToStr(right) << std::endl;
        std::cout << "up: " << AxisToStr(up) << std::endl;
        std::cout << "front: " << AxisToStr(front) << std::endl;
    }
};

struct Settings
{
    double scaleFactor;
    AxisSetup origAxes;
    AxisSetup convAxes;
};
 
}
}
}

#endif
