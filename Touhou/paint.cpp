#include <Windows.h>
#include <atlimage.h>

#include "entity.h"
#include "globalVariable.h"
#include "tools.h"

bool bPLeft = false, bPRight = false, bPUp = false, bPDown = false;

void DrawLeftPanel(HDC mdc);

void GameCheck() {
    // Dead Enemy Check
    auto deadenemy = EnemyDead.begin();
    while (deadenemy != EnemyDead.end()) {
        if (deadenemy->second > 7) {
            deadenemy->first->bCanBeDeleted = true;
            deadenemy = EnemyDead.erase(deadenemy);
        } else {
            ++deadenemy;
        }
    }

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
    auto it = EnemyExists.begin();
    while (it != EnemyExists.end()) {
        Bullet *tmpBullet = (*it)->getBullet();
        if (!(*it)->isDead()) {
            // PlayerBullet To Enemy
            auto itpl = tmpplayerbullet->ptPos.begin();
            while (itpl != tmpplayerbullet->ptPos.end()) {
                if (R2R((*it)->ix, (*it)->iy, (*it)->iWidth - 30, (*it)->iHeight, itpl->x, itpl->y,
                        tmpplayerbullet->iWidth, tmpplayerbullet->iHeight)) {
                    if ((*it)->getHealth() <= player.BulletDamage) {
                        (*it)->changeDead();
                        EnemyDead.push_back(std::make_pair(*it, 0));
                        break;
                    } else {
                        (*it)->changeHealth(-player.BulletDamage);
                    }
                    itpl = tmpplayerbullet->ptPos.erase(itpl);
                } else {
                    ++itpl;
                }
            }
            // Enemy To Player
            if (bShift) {
                if (C2R(player.ix, player.iy, player.iRadius, (*it)->ix, (*it)->iy, (*it)->iWidth - 30,
                        (*it)->iHeight - 20)) {
                    fail = true;
                    win = false;
                    running = false;
                    return;
                }
            } else {
                if (R2R((*it)->ix, (*it)->iy, (*it)->iWidth - 30, (*it)->iHeight - 20, player.ix, player.iy,
                        player.iWidth - 40, player.iHeight - 40)) {
                    fail = true;
                    win = false;
                    running = false;
                    return;
                }
            }
        } else {
            if (tmpBullet->ptPos.empty() && (*it)->bCanBeDeleted) {
                it = EnemyExists.erase(it);
                continue;
            }
        }

        /*
        // EnemyBullet To Player
        if (bShift) {
            for (auto itbullet = tmpBullet->ptPos.begin(); itbullet != tmpBullet->ptPos.end(); ++itbullet) {
                if (C2C(itbullet->x, itbullet->y, tmpBullet->iRadius, player.ix, player.iy, player.iRadius)) {
                    fail = true;
                    running = false;
                    return;
                }
            }
        } else {
            for (auto itbullet = tmpBullet->ptPos.begin(); itbullet != tmpBullet->ptPos.end(); ++itbullet) {
                if (C2R(itbullet->x, itbullet->y, tmpBullet->iRadius, player.ix, player.iy, player.iWidth - 40,
                        player.iHeight - 40)) {
                    fail = true;
                    running = false;
                    return;
                }
            }
        }
        */

        ++it;
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

    /*  Òò²âÊÔ×¢ÊÍ */
    // Enemy Bullet Spawn Check
    for (auto it = EnemyExists.begin(); it != EnemyExists.end(); ++it) {
        (*it)->bulletMoving();
        if (!(*it)->isDead()) {
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
}

void GamePaint(HDC hdc) {
    HDC mdc = ciScreen.GetDC();

    // Draw Background
    ciTitleBk.Draw(mdc, 0, 0, 1024, 768, 0, 0, ciTitleBk.GetWidth(), ciTitleBk.GetHeight());

    // Draw LeftPanem
    DrawLeftPanel(mdc);

    // Draw RightPanel
    // TODO

    // Done
    ciScreen.ReleaseDC();
    ciScreen.Draw(hdc, rect);

    tPre = GetTickCount64();
}

void MenuPaint(HDC hdc) {
    HDC mdc = ciScreen.GetDC();
    if (win || fail) {
        // Draw Background
        ciTitleBk.Draw(mdc, 0, 0, 1024, 768, 0, 0, ciTitleBk.GetWidth(), ciTitleBk.GetHeight());
        // Draw LeftPanem
        DrawLeftPanel(mdc);

        if (fail) {
            ciFail.Draw(mdc, GWidth / 2 - ciFail.GetWidth() * ZOOM / 2, 100, ciFail.GetWidth() * ZOOM,
                        ciFail.GetHeight() * ZOOM);
        } else {
            ciWin.Draw(mdc, GWidth / 2 - ciWin.GetWidth() / 2, 100, ciWin.GetWidth(), ciWin.GetHeight());
        }

        ciRetry.Draw(mdc, GWidth / 2 - ciRetry.GetWidth() / 2, GHeight / 2 - ciRetry.GetHeight() / 2,
                     ciRetry.GetWidth(), ciRetry.GetHeight());
    } else {
        ciTitleBk0.Draw(mdc, 0, 0, 1024, 768, 0, 0, ciTitleBk.GetWidth(), ciTitleBk.GetHeight());
        ciTitle.Draw(mdc, 30, 30);
        if (iMx > 50 && iMx < -50 + ciGameStart.GetWidth() / 2 && iMy > 400 && iMy < 400 + ciGameStart.GetHeight()) {
            SetCursor(::LoadCursor(nullptr, IDC_HAND));
            ciGameStart.Draw(mdc, 50, 400, ciGameStart.GetWidth() / 2, ciGameStart.GetHeight(),
                             ciGameStart.GetWidth() / 2, 0, ciGameStart.GetWidth() / 2, ciGameStart.GetHeight());
        } else {
            ciGameStart.Draw(mdc, 50, 400, ciGameStart.GetWidth() / 2, ciGameStart.GetHeight(), 0, 0,
                             ciGameStart.GetWidth() / 2, ciGameStart.GetHeight());
        }
    }

    // Done
    ciScreen.ReleaseDC();
    ciScreen.Draw(hdc, rect);
    tPre = GetTickCount64();
}

void MenuCheck() {
    if (win || fail) {
        if (LBdown) {
            if (iMx > GWidth / 2 - ciRetry.GetWidth() / 2 && iMx < GWidth / 2 + ciRetry.GetWidth() / 2 &&
                iMy > GHeight / 2 - ciRetry.GetHeight() / 2 && iMy < GHeight / 2 + ciRetry.GetHeight() / 2) {
                running = true;
                GameTurn = 0;
                stage1ini = true;
            }
        }
    } else {
        if (LBdown) {
            if (iMx > 50 && iMx < -50 + ciGameStart.GetWidth() / 2 && iMy > 400 &&
                iMy < 400 + ciGameStart.GetHeight()) {
                running = true;
                GameTurn = 0;
                stage1ini = true;
            }
        }
    }
}

// Draw Panel
void DrawLeftPanel(HDC mdc) {
    // Draw LeftPanel
    HDC leftpanel = ciLeftPanel.GetDC();
    ciBackground.Draw(leftpanel, 0, iBackgroundOff);
    ciBackground.Draw(leftpanel, 0, 0, ciBackground.GetWidth(), iBackgroundOff, 0,
                      ciScreen.GetHeight() - iBackgroundOff, ciBackground.GetWidth(), iBackgroundOff);
    iBackgroundOff = (iBackgroundOff + 8) % GHeight;

    // Draw Enemy
    for (auto it = EnemyExists.begin(); it != EnemyExists.end(); ++it) {
        if (!(*it)->isDead()) {
            (*it)->draw(leftpanel);
        }
    }

    // Draw Break Effect
    for (auto it = EnemyDead.begin(); it != EnemyDead.end(); ++it) {
        if (it->second < 4) {
            ciBreak.Draw(leftpanel, it->first->ix - 32, it->first->iy - 32, 64, 64, it->second * 64, 0, 64, 64);
        } else {
            ciBreak.Draw(leftpanel, it->first->ix - 32, it->first->iy - 32, 64, 64, (it->second % 4) * 64, 64, 64, 64);
        }
        ++(it->second);
    }

    // Draw Player Bullet
    player.BulletPlayer.draw(leftpanel);

    // Draw Player
    player.draw(leftpanel);  // Last draw
    if (bShift) {
        ciEnemyBullet2.Draw(leftpanel, player.ix - player.iRadius, player.iy - player.iRadius, player.iRadius * 2,
                            player.iRadius * 2, 0, 0, 16, 16);
    }

    // Draw Enemy Bullet
    for (auto it = EnemyExists.begin(); it != EnemyExists.end(); ++it) {
        (*it)->getBullet()->draw(leftpanel);
    }

    // LeftPanel Done
    ciLeftPanel.ReleaseDC();
    ciLeftPanel.Draw(mdc, 0, 0);
}