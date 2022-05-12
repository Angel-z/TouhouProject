#include <Windows.h>
#include <atlimage.h>

#include "entity.h"
#include "globalVariable.h"
#include "tools.h"

bool bPLeft = false, bPRight = false, bPUp = false, bPDown = false;

void GameCheck() {
    // Player Moving Check
    if (GetAsyncKeyState(VK_UP)) {
        if (!GetAsyncKeyState(VK_DOWN)) {
            if (player.ptLeftTop.y - iPlayerSpeed < 0) {
                player.ptLeftTop.y = 0;
                player.iy = player.iHeight / 2;
            } else {
                player.yChange(-iPlayerSpeed);
            }
            bPUp = true;
            bPDown = false;
        } else {
            bPUp = false;
            bPDown = false;
        }
        
    } else if (GetAsyncKeyState(VK_DOWN)) {
        if (player.ptLeftTop.y + player.iHeight + iPlayerSpeed > GHeight) {
            player.ptLeftTop.y = GHeight - player.iHeight;
            player.iy = GHeight - player.iHeight / 2;
        } else {
            player.yChange(iPlayerSpeed);
        }
        bPUp = false;
        bPDown = true;
    } else {
        bPUp = false;
        bPDown = false;
    }
    if (GetAsyncKeyState(VK_LEFT)) {
        if (!GetAsyncKeyState(VK_RIGHT)) {
            if (player.ptLeftTop.x - iPlayerSpeed < 0) {
                player.ptLeftTop.x = 0;
                player.ix = player.iWidth / 2;
            } else {
                player.xChange(-iPlayerSpeed);
            }
            bPLeft = true;
            bPRight = false;
        } else {
            bPLeft = false;
            bPRight = false;
        }
        
    } else if (GetAsyncKeyState(VK_RIGHT)) {
        if (player.ptLeftTop.x + player.iWidth + iPlayerSpeed > GWidth) {
            player.ptLeftTop.x = GWidth - player.iWidth;
            player.ix = GWidth - player.iWidth / 2;
        } else {
            player.xChange(iPlayerSpeed);
        }
        bPLeft = false;
        bPRight = true;
    } else {
        bPLeft = false;
        bPRight = false;
    }

    // Bullet Check
    player.bulletMoving();
    Bullet *tmpBullet = nullptr;
    if (GetAsyncKeyState(0x5A)) {
        tmpBullet = player.getBullet();
        if (tNow - tmpBullet->msLastShoot > tmpBullet->msBulletCold) {
            player.bulletStatusChange(true);
        }
        if (tmpBullet->bulletUsable) {
            player.bulletSpawn();
            player.bulletStatusChange(false);
        }
    }

}

void MyPaint(HDC hdc) {
    HDC mdc = ciScreen.GetDC();

    // Draw Background
    ciTitleBk.Draw(mdc, 0, 0, 1024, 768, 0, 0, ciTitleBk.GetWidth(), ciTitleBk.GetHeight());
    ciBackground.Draw(mdc, 0, iBackgroundOff);
    ciBackground.Draw(mdc, 0, 0, ciBackground.GetWidth(), iBackgroundOff, 0, ciScreen.GetHeight() - iBackgroundOff,
                      ciBackground.GetWidth(), iBackgroundOff);
    iBackgroundOff = (iBackgroundOff + 8) % GHeight;

    // Draw Bullet
    player.BulletPlayer.draw(mdc);

    // Draw Entity
    player.draw(mdc, player.ptLeftTop.x, player.ptLeftTop.y);
    ciScreen.ReleaseDC();
    ciScreen.Draw(hdc, rect);

    tPre = GetTickCount64();
}