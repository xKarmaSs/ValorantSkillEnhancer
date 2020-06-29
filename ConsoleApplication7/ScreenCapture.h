#pragma once
#include <iostream>
#include "windows.h"
#include <vector>

class ScreenCapture
{
private:
	HWND _hwnd;
	int _width;
	int _height;

public:
	BYTE* ScreenData;
	ScreenCapture(HWND hwnd);
	void screenshot();
	int getWidth() const {
		return _width;
	}

	int getHeight() const {
		return _height;
	}

	std::vector<int> getRGB(int x, int y) const;


};

