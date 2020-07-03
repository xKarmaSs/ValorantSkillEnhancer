#pragma once
#include <iostream>
#include "windows.h"
#include <vector>

class ScreenCapture
{
private:
	HWND _hwnd;
	HDC _hdc;
	HDC _hdcTemp;
	HBITMAP _bmTemp;
	int _width;
	int _height;
	long _lastSSTime;
	int _delay;
	int _dimension;

public:
	BYTE* ScreenData;
	ScreenCapture(HWND hwnd, int delay);
	void screenshot();
	int getWidth() const {
		return _width;
	}

	int getHeight() const {
		return _height;
	}
	~ScreenCapture() {
		ReleaseDC(NULL, _hdc);
		DeleteDC(_hdcTemp);
		DeleteObject(_bmTemp);
	}

	ScreenCapture(HWND hwnd, int delay, int FOV);

	std::vector<int> getRGB(int x, int y) const;


};

