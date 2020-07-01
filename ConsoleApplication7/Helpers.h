#pragma once
#include <iostream>
#include <chrono>
#include "windows.h"
#include <math.h>   

inline long getUnixTime() {
    auto now = std::chrono::system_clock::now();
 
    return std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
}

inline float distanceFromCrosshair(int x, int y) {
    return sqrt((float)x * x + y * y);
}

inline int roundtosignedchar(int a)
{
    if (a < -127)
    {
        return -127;
    }
    else if (a > 127)
    {
        return 127;
    }
    else
    {
        return a;
    }
}