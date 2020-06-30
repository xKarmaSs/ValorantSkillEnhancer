#pragma once
#include <iostream>
#include <chrono>
#include "windows.h"

inline long getUnixTime() {
    auto now = std::chrono::system_clock::now();
 
    return std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
}