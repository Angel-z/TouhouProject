#include <Windows.h>
#include <atlimage.h>

#include "globalVariable.h"
#include "tools.h"

int iMx, iMy;
bool LBdown = false, bShift = false;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_KEYDOWN: {
            if (wParam == VK_SHIFT) {
                bShift = true;
                iPlayerSpeed = 7;
            }
        } break;
        case WM_KEYUP: {
            if (wParam == VK_SHIFT) {
                bShift = false;
                iPlayerSpeed = 15;
            }
        } break;
        case WM_MOUSEMOVE: {
            iMx = LOWORD(lParam);
            iMy = HIWORD(lParam);
        } break;
        case WM_LBUTTONDOWN: {
            LBdown = true;
        } break;
        case WM_LBUTTONUP: {
            LBdown = false;
        } break;
        case WM_DESTROY:
            ReleaseDC(hwnd, hdc);
            ciScreen.Destroy();
            ciBackground.Destroy();
            ciPlayer.Destroy();
            ciTitleBk.Destroy();
            ciPlayerBullet.Destroy();
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
            break;
    }

    return 0;
}
