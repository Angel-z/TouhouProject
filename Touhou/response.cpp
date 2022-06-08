#include <Windows.h>
#include <atlimage.h>

#include "globalVariable.h"
#include "tools.h"

int iMx, iMy;
bool LBdown = false, bShift = false;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_KEYDOWN: {
            if (running && wParam == VK_SHIFT) {
                bShift = true;
                iPlayerSpeed = 7;
            } else if (wParam == VK_ESCAPE) {
                running = false;
                win = false;
                fail = false;
            } else if (wParam == 'R') {
                GameTurn = 0;
                stage1ini = true;
                win = false;
                fail = false;
                running = false;
                replaying = true;
                recordPos = 0;
            }
        } break;
        case WM_KEYUP: {
            if (running && wParam == VK_SHIFT) {
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
        case WM_DESTROY: {
            ReleaseDC(hwnd, hdc);
            ciScreen.Destroy();
            ciBackground.Destroy();
            ciTitleBk0.Destroy();
            ciTitleBk.Destroy();
            ciGameStart.Destroy();
            ciBreak.Destroy();
            ciLeftPanel.Destroy();
            ciRightPanel.Destroy();
            ciWin.Destroy();
            ciFail.Destroy();
            ciRetry.Destroy();
            ciTitle.Destroy();
            ciCharactor.Destroy();
            ciPlayer.Destroy();
            ciPlayerBullet.Destroy();
            ciEnemy1.Destroy();
            ciEnemyBullet.Destroy();
            ciEnemy2.Destroy();
            ciEnemyBullet2.Destroy();
            DeleteObject(normalFont);

            PostQuitMessage(0);
        } break;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
            break;
    }

    return 0;
}
