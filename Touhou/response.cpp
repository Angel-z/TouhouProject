#include <Windows.h>
#include <atlimage.h>

#include "globalVariable.h"
#include "tools.h"

bool bShoot = false;

int iMx, iMy;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_KEYDOWN: {
        } break;
        case WM_KEYUP: {
        } break;
        case WM_MOUSEMOVE: {
            iMx = LOWORD(lParam);
            iMy = HIWORD(lParam);
        } break;
        case WM_DESTROY:
            ciScreen.Destroy();
            ciBackground.Destroy();
            ciPlayer.Destroy();
            ciTitleBk.Destroy();
            ciPlayerBullet.Destroy();
            ReleaseDC(hwnd, hdc);
            //
            delete *EnemyExists.begin();
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
            break;
    }

    return 0;
}
