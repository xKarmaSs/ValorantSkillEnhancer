#pragma once

#define TRIGGERBOT_WINDOW 5 // within 5 pixels radius of the crosshair
#define SHOOTING_DELAY 50 // delay between shots
#define TRIGGER_DELAY 10 // delay after crossing an enemy
#define HEAD_OFFSET_X 2
#define HEAD_OFFSET_Y 3
#define FIRE_HOTKEY VK_F12
#define TRIGGERBOT_HOTKEY VK_F10
#define AIMBOT_HOTKEY VK_MENU
#define SS_PER_SEC 500 // how many ss to take per sec
#define AIMBOT_FOV 10 // within x pixels away from the crosshair
#define WIDTH 1920
#define HEIGHT 1080
#define ADRUINO_PORT "COM7"
#define ADRUINO_MOUSE_CLICK_SIGNAL_1 0x80
#define ADRUINO_MOUSE_CLICK_SIGNAL_2 0x80

const int RED_RANGE[] = {200, 255};
const int GREEN_RANGE[] = { 0, 200 };
const int BLUE_RANGE[] = { 200, 255 };

const float RED_DOMINANCE_THRES[] = { 0.8, 1.0 };
const float GREEN_DOMINANCE_THRES[] = { 0.0, 0.80 };
const float BLUE_DOMINANCE_THRES[] = { 0.5, 1.0 };