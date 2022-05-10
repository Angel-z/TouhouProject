#include <Windows.h>
#include <atlimage.h>

#include "globalVariable.h"
#include "tools.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_KEYDOWN: {
        } break;
        case WM_KEYUP: {
        } break;
        case WM_MOUSEMOVE: {
        } break;
        case WM_DESTROY:
            CIscreen.Destroy();
            ReleaseDC(hwnd, hdc);
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
            break;
    }

    return 0;
}
