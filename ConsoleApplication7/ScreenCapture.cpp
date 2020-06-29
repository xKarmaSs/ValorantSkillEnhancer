#include "ScreenCapture.h"

ScreenCapture::ScreenCapture(HWND hwnd) : _hwnd(), _width(), _height(), ScreenData(NULL) {
    _hwnd = hwnd;
    HDC hdc = GetDC(_hwnd);
    _width = GetDeviceCaps(hdc, HORZRES);
    _height = GetDeviceCaps(hdc, VERTRES);
    ScreenData = new BYTE[4 * _width * _height];
    ReleaseDC(NULL, hdc);
}

std::vector<int> ScreenCapture::getRGB(int x, int y) const
{
    return { ScreenData[4 * ((y * _width) + x)], 
        ScreenData[4 * ((y * _width) + x) + 1], 
        ScreenData[4 * ((y * _width) + x) + 2] };
}

void ScreenCapture::screenshot()
{
    HDC hdc = GetDC(_hwnd);
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, _width, _height);
    SelectObject(hdcMem, hBitmap);
    BitBlt(hdcMem, 0, 0, _width, _height, hdc, 0, 0, SRCCOPY);
    BITMAPINFOHEADER bmi = { 0 };
    bmi.biSize = sizeof(BITMAPINFOHEADER);
    bmi.biPlanes = 1;
    bmi.biBitCount = 32;
    bmi.biWidth = _width;
    bmi.biHeight = -_height;
    bmi.biCompression = BI_RGB;
    bmi.biSizeImage = 0;
    GetDIBits(hdcMem, hBitmap, 0, _height, ScreenData, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);
    DeleteDC(hdcMem);
    DeleteObject(hBitmap);
    ReleaseDC(NULL, hdc);
}
