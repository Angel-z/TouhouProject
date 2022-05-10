#include <Windows.h>
#include <atlimage.h>
#pragma comment(lib, "MSIMG32.LIB")

//extern HBITMAP hcharactor, hbackground;
extern HDC hdc, mdc, bufferdc;
extern HWND hwnd;
extern ULONGLONG tPre, tNow;

extern HBITMAP fullmap;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_KEYDOWN: {
        } break;
        case WM_KEYUP: {
        } break;
        case WM_MOUSEMOVE: {
        } break;
        case WM_DESTROY:
            //DeleteObject(hbackground);
            //DeleteObject(hcharactor);
            DeleteDC(mdc);
            DeleteDC(bufferdc);
            ReleaseDC(hwnd, hdc);
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
            break;
    }

    return 0;
}

void MyPaint(HDC hdc) {
    RECT rect;
    GetClientRect(hwnd, &rect);


    tPre = GetTickCount64();
}