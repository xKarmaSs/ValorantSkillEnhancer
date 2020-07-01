#include "ScreenCapture.h"
#include "Helpers.h"
#include "Settings.h"

ScreenCapture::ScreenCapture(HWND hwnd, int delay, int dimension) : _width(), _height(), ScreenData(NULL) {
    _hdc = GetDC(_hwnd);
    _hdcTemp = CreateCompatibleDC(_hdc);
    _width = GetDeviceCaps(_hdc, HORZRES);
    _height = GetDeviceCaps(_hdc, VERTRES);
    ScreenData = new BYTE[4 * _width * _height];
    _dimension = dimension;
    _delay = delay;
}

std::vector<int> ScreenCapture::getRGB(int x, int y) const
{
    return { ScreenData[4 * ((y * _width) + x)], 
        ScreenData[4 * ((y * _width) + x) + 1], 
        ScreenData[4 * ((y * _width) + x) + 2] };
}

void ScreenCapture::screenshot()
{
    if (getUnixTime() - _lastSSTime < _delay)
        Sleep(_delay - getUnixTime() + _lastSSTime);

    _lastSSTime = getUnixTime();
    int centerX = getWidth() / 2;
    int centerY = getHeight() / 2;
    HBITMAP hBitmap = CreateCompatibleBitmap(_hdc, _width, _height);
    SelectObject(_hdcTemp, hBitmap);
    BitBlt(_hdcTemp, centerX - _dimension, centerY - _dimension, _dimension *2, _dimension *2, _hdc, centerX - _dimension, centerY - _dimension, SRCCOPY);
    BITMAPINFOHEADER bmi = { 0 };
    bmi.biSize = sizeof(BITMAPINFOHEADER);
    bmi.biPlanes = 1;
    bmi.biBitCount = 32;
    bmi.biWidth = _width;
    bmi.biHeight = -_height;
    bmi.biCompression = BI_RGB;
    bmi.biSizeImage = 0;
    GetDIBits(_hdcTemp, hBitmap, centerY - _dimension, centerY + _dimension, ScreenData, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);
    DeleteObject(hBitmap);
}
