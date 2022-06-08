#include <Windows.h>

#include <fstream>

#include "entity.h"
#include "globalVariable.h"
#include "tools.h"

bool replaying = false;
std::streampos recordPos = 0;

// 调用前先删除已存在文件
void Record() {
    std::fstream file;

    file.open("record.dat", std::ios::out | std::ios::app);
    file << bPLeft << " " << bPRight << " " << bPUp << " " << bPDown << " " << bShift << " " << bPShoot << " " << win
         << "\n";
    file.close();
}

void Replay() {
    std::fstream file;

    file.open("record.dat", std::ios::in);

    file.seekg(recordPos);
    file >> bPLeft >> bPRight >> bPUp >> bPDown >> bShift >> bPShoot >> win;
    if (file.eof()) {
        if (!win) {
            fail = true;
        }
        running = false;
        replaying = false;
        return;
    }
    recordPos = file.tellg();
    file.close();

    if (bShift) {
        iPlayerSpeed = 7;
    } else {
        iPlayerSpeed = 15;
    }

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
    if (bPUp) {
        if (!bPDown) {
            if (player.ptLeftTop.y - iPlayerSpeed < 0) {
                player.ptLeftTop.y = 0;
                player.iy = player.iHeight / 2;
            } else {
                player.yChange(-iPlayerSpeed);
            }
        }
    } else if (bPDown) {
        if (player.ptLeftTop.y + player.iHeight + iPlayerSpeed > GHeight) {
            player.ptLeftTop.y = GHeight - player.iHeight;
            player.iy = GHeight - player.iHeight / 2;
        } else {
            player.yChange(iPlayerSpeed);
        }
    }
    if (bPLeft) {
        if (!bPRight) {
            if (player.ptLeftTop.x - iPlayerSpeed < 0) {
                player.ptLeftTop.x = 0;
                player.ix = player.iWidth / 2;
            } else {
                player.xChange(-iPlayerSpeed);
            }
        }
    } else if (bPRight) {
        if (player.ptLeftTop.x + player.iWidth + iPlayerSpeed > GWidth) {
            player.ptLeftTop.x = GWidth - player.iWidth;
            player.ix = GWidth - player.iWidth / 2;
        } else {
            player.xChange(iPlayerSpeed);
        }
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
                    win = false;
                    running = false;
                    return;
                }
            }
        } else {
            for (auto itbullet = tmpBullet->ptPos.begin(); itbullet != tmpBullet->ptPos.end(); ++itbullet) {
                if (C2R(itbullet->x, itbullet->y, tmpBullet->iRadius, player.ix, player.iy, player.iWidth - 40,
                        player.iHeight - 40)) {
                    fail = true;
                    win = false;
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
    if (bPShoot) {
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