// ConsoleApplication7.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "windows.h"
#include "ScreenCapture.h"
#include "EnemyScanner.h"
#include <thread>
#include <chrono>

bool isOn = false;
void foo(ScreenCapture *sc) {
    while (true) {
        if (isOn) {
            sc->screenshot();
        }
        Sleep(1);
    }

}

bool isKeyPressed(int key) {
    SHORT keyState = GetKeyState(key);
    bool isDown = keyState & 0x8000;
    return isDown;
}

int main()
{
    ScreenCapture sc(NULL);
    EnemyScanner enemyScan(sc);
    INPUT input;
    WORD vkey = FIRE_HOTKEY;
    input.type = INPUT_KEYBOARD;
    input.ki.wScan = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;
    input.ki.wVk = vkey;
    input.ki.dwFlags = 0; // there is no KEYEVENTF_KEYDOWN

    //std::thread thread_obj(foo, &sc); // multi thread for possible performance boost
    auto last = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());

    while (true) {
        auto current = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());

        //delay in between shots
        if ((current - last).count() < (long) SHOOTING_DELAY)
            Sleep((long) SHOOTING_DELAY - (current - last).count());
        
        if (isKeyPressed(TRIGGERBOT_HOTKEY)) {
            enemyScan.update();
            if (enemyScan.isEnemyAtCrosshair()) {
                SendInput(1, &input, sizeof(INPUT));
                input.ki.dwFlags = KEYEVENTF_KEYUP;
                SendInput(1, &input, sizeof(INPUT));
                input.ki.dwFlags = 0;
                last = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
            }
        }
        Sleep(1);
    }
    //thread_obj.join();
}