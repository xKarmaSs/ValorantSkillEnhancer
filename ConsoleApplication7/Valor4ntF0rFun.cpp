// ConsoleApplication7.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <thread>
#include <chrono>

#include "windows.h"
#include "ScreenCapture.h"
#include "EnemyScanner.h"
#include "Settings.h"
#include "Helpers.h"

bool isOn = false;
void foo(EnemyScanner *sc) {
    while (true) {
        if (isOn) {
            sc->update();
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
    ScreenCapture sc(NULL, 1000 / SS_PER_SEC);
    EnemyScanner enemyScan(sc);
    INPUT input;
    WORD vkey = FIRE_HOTKEY;
    input.type = INPUT_KEYBOARD;
    input.ki.wScan = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;
    input.ki.wVk = vkey;
    input.ki.dwFlags = 0; // there is no KEYEVENTF_KEYDOWN

    //std::thread thread_obj(foo, &enemyScan); // multi thread for possible performance boost
    long last = getUnixTime();
   
    while (true) {
        long current = getUnixTime();

        //delay in between shots
        if (current < (long) SHOOTING_DELAY)
            Sleep((long) SHOOTING_DELAY - (current - last));
        
        isOn = false;
        if (isKeyPressed(TRIGGERBOT_HOTKEY)) {
            isOn = true;
            enemyScan.update();
            if (enemyScan.isEnemyAtCrosshair()) {
                Sleep(TRIGGER_DELAY);
                SendInput(1, &input, sizeof(INPUT));
                input.ki.dwFlags = KEYEVENTF_KEYUP;
                SendInput(1, &input, sizeof(INPUT));
                input.ki.dwFlags = 0;
                last = getUnixTime();
            }
        } 

        Sleep(1);
    }
    //thread_obj.join();
}