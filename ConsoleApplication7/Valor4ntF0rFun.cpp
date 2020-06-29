// ConsoleApplication7.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "windows.h"
#include "ScreenCapture.h"
#include "EnemyScanner.h"
#include <thread>

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
    WORD vkey = VK_F12;
    input.type = INPUT_KEYBOARD;
    input.ki.wScan = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;
    input.ki.wVk = vkey;
    input.ki.dwFlags = 0; // there is no KEYEVENTF_KEYDOWN
    //std::thread thread_obj(foo, &sc);
    while (true) {
        if (isKeyPressed(VK_F10)) {
            enemyScan.update();
            if (enemyScan.isEnemyAtCrosshair()) {
                SendInput(1, &input, sizeof(INPUT));
                input.ki.dwFlags = KEYEVENTF_KEYUP;
                SendInput(1, &input, sizeof(INPUT));
                input.ki.dwFlags = 0;
            }
        }
        Sleep(1);
    }
    //thread_obj.join();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
