#include "ScreenCapture.h"

ScreenCapture::ScreenCapture(HWND hwnd) : _width(), _height(), ScreenData(NULL) {
    _hdc = GetDC(_hwnd);
    _hdcTemp = CreateCompatibleDC(_hdc);
    _width = GetDeviceCaps(_hdc, HORZRES);
    _height = GetDeviceCaps(_hdc, VERTRES);
    ScreenData = new BYTE[4 * _width * _height];
}

std::vector<int> ScreenCapture::getRGB(int x, int y) const
{
    return { ScreenData[4 * ((y * _width) + x)], 
        ScreenData[4 * ((y * _width) + x) + 1], 
        ScreenData[4 * ((y * _width) + x) + 2] };
}

void ScreenCapture::screenshot()
{
    HBITMAP hBitmap = CreateCompatibleBitmap(_hdc, _width, _height);
    SelectObject(_hdcTemp, hBitmap);
    BitBlt(_hdcTemp, 0, 0, _width, _height, _hdc, 0, 0, SRCCOPY);
    BITMAPINFOHEADER bmi = { 0 };
    bmi.biSize = sizeof(BITMAPINFOHEADER);
    bmi.biPlanes = 1;
    bmi.biBitCount = 32;
    bmi.biWidth = _width;
    bmi.biHeight = -_height;
    bmi.biCompression = BI_RGB;
    bmi.biSizeImage = 0;
    GetDIBits(_hdcTemp, hBitmap, 0, _height, ScreenData, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);
    DeleteObject(hBitmap);
}
