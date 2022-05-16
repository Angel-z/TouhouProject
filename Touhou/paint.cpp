#include <Windows.h>
#include <atlimage.h>
#include <math.h>

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

    // Collision Check
    Bullet *tmpplayerbullet = player.getBullet();
    for (auto it = EnemyExists.begin(); it != EnemyExists.end(); ++it) {
        Bullet *tmpBullet = (*it)->getBullet();
        // PlayerBullet To Enemy
        for (auto itpl = tmpplayerbullet->ptPos.begin(); itpl != tmpplayerbullet->ptPos.end(); ++itpl) {
            if (R2R((*it)->ix, (*it)->iy, (*it)->iWidth - 30, (*it)->iHeight, itpl->x, itpl->y, tmpplayerbullet->iWidth,
                    tmpplayerbullet->iHeight)) {
                MessageBox(hwnd, "attacked", "enemydead", MB_OK);
            }
        }
        // EnemyBullet To Player
        for (auto itbullet = tmpBullet->ptPos.begin(); itbullet != tmpBullet->ptPos.end(); ++itbullet) {
            if (C2R(itbullet->x, itbullet->y, tmpBullet->iRadius, player.ix, player.iy, player.iWidth - 40,
                    player.iHeight - 30)) {
                MessageBox(hwnd, "bulleted", "playerdead", MB_OK);
            }
        }
        // Enemy To Player
        if (R2R((*it)->ix, (*it)->iy, (*it)->iWidth - 30, (*it)->iHeight - 20, player.ix, player.iy, player.iWidth - 40,
                player.iHeight - 30)) {
            MessageBox(hwnd, "enemy", "playerdead", MB_OK);
        }
    }

    // Player Bullet Spawn Check
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

    // Enemy Bullet Spawn Check
    for (auto it = EnemyExists.begin(); it != EnemyExists.end(); ++it) {
        (*it)->bulletMoving();
        tmpBullet = (*it)->getBullet();
        if (tNow - tmpBullet->msLastShoot > tmpBullet->msBulletCold) {
            (*it)->bulletStatusChange(true);
        }
        if (tmpBullet->bulletUsable) {
            (*it)->bulletSpawn();
            (*it)->bulletStatusChange(false);
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
    for (auto it = EnemyExists.begin(); it != EnemyExists.end(); ++it) {
        (*it)->getBullet()->draw(mdc);
    }

    // test
    for (auto it = EnemyExists.begin(); it != EnemyExists.end(); ++it) {
        (*it)->draw(mdc);
    }
    // auto e1 = EnemyExists.begin();
    //(*e1)->draw(mdc);

    // Draw Entity
    player.draw(mdc);  // Last draw
    // ciEnemyBullet2.Draw(mdc, player.ix - player.iRadius, player.iy - player.iRadius, player.iRadius * 2,
    //                    player.iRadius * 2, 0, 0, 16, 16);

    // Done
    ciScreen.ReleaseDC();
    ciScreen.Draw(hdc, rect);

    tPre = GetTickCount64();
}