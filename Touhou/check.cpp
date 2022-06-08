#include <Windows.h>

#include "entity.h"
#include "globalVariable.h"
#include "tools.h"

bool bPLeft = false, bPRight = false, bPUp = false, bPDown = false;
bool bPShoot = false;

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
                        score += 1000;
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

        ++it;
    }

    // Player Bullet Spawn Check
    player.bulletMoving();
    Bullet *tmpBullet = nullptr;
    if (GetAsyncKeyState(0x5A)) {
        bPShoot = true;
        tmpBullet = player.getBullet();
        if (tNow - tmpBullet->msLastShoot > tmpBullet->msBulletCold) {
            player.bulletStatusChange(true);
        }
        if (tmpBullet->bulletUsable) {
            player.bulletSpawn();
            player.bulletStatusChange(false);
        }
    } else {
        bPShoot = false;
    }

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

void MenuCheck() {
    if (win || fail) {
        if (LBdown) {
            if (iMx > GWidth / 2 - ciRetry.GetWidth() / 2 && iMx < GWidth / 2 + ciRetry.GetWidth() / 2 &&
                iMy > GHeight / 2 - ciRetry.GetHeight() / 2 && iMy < GHeight / 2 + ciRetry.GetHeight() / 2) {
                running = true;
                GameTurn = 0;
                stage1ini = true;
                win = false;
                fail = false;
                remove("record.dat");
            }
        }
    } else {
        if (LBdown) {
            if (iMx > 50 && iMx < -50 + ciGameStart.GetWidth() / 2 && iMy > 400 &&
                iMy < 400 + ciGameStart.GetHeight()) {
                running = true;
                GameTurn = 0;
                stage1ini = true;
                win = false;
                fail = false;
                remove("record.dat");
            }
        }
    }
}
