#include "entity.h"

#include <atlimage.h>

#include <vector>

#include "globalVariable.h"

// Entity
Entity::Entity(int _x, int _y, int _iFileWidth, int _iFileHeight, int _iFileRadius) {
    ix = _x;
    iy = _y;
    iFileWidth = _iFileWidth;
    iFileHeight = _iFileHeight;
    iWidth = iFileWidth * ZOOM;
    iHeight = iFileHeight * ZOOM;
    iFileRadius = _iFileRadius;
    iRadius = iFileRadius * ZOOM;
    ptLeftTop.x = ix - iWidth / 2;
    ptLeftTop.y = iy - iHeight / 2;

    bCanBeDeleted = false;
}

void Entity::draw(HDC hdc) {}
void Entity::bulletStatusChange(bool stat) {}
Bullet *Entity::getBullet() {
    return nullptr;
}
void Entity::bulletSpawn() {}
void Entity::bulletMoving() {}
bool Entity::isDead() {
    return false;
}
void Entity::changeDead() {}

int Entity::getHealth() {
    return 0;
}

void Entity::changeHealth(int health) {}

void Entity::changeDir(int _dir) {}

int Entity::getDir() {
    return -1;
}

void Entity::xChange(int x) {
    ix += x;
    ptLeftTop.x += x;
}

void Entity::yChange(int y) {
    iy += y;
    ptLeftTop.y += y;
}

void Entity::xySet(int x, int y) {
    ix = x;
    iy = y;
    ptLeftTop.x = ix - iWidth / 2;
    ptLeftTop.y = iy - iHeight / 2;
}

// Bullet
Bullet::Bullet(CImage &_ci, int _iFileWidth, int _iFileHeight, int _msBulletCold, int _iFileRadius)
    : Entity(0, 0, _iFileWidth, _iFileHeight, _iFileRadius), ci(_ci) {
    bulletUsable = true;
    msBulletCold = _msBulletCold;
    msLastShoot = 0;
}

void Bullet::draw(HDC hdc) {
    for (auto it = ptPos.begin(); it < ptPos.end(); ++it) {
        ci.Draw(hdc, it->x - iWidth / 2, it->y - iHeight / 2, iWidth, iHeight);
    }
}

// Player
CImage &Player::ci = ciPlayer;

Player::Player() : Entity(0, 0, 32, 48, 2), BulletPlayer(ciPlayerBullet, 10, 15, 60, 0) {
    ix = GWidth / 2 - iWidth / 2;
    iy = GHeight - iHeight / 2;
    ptLeftTop.x = ix - iWidth / 2;
    ptLeftTop.y = iy - iHeight / 2;

    iUpDownIndex = 0;
    iLeftIndex = 0;
    iRightIndex = 0;

    BulletDamage = 100;
}

void Player::draw(HDC hdc) {
    if (bPLeft) {
        iUpDownIndex = 0;
        iRightIndex = 0;
        if (iLeftIndex == 8) {
            iLeftIndex = 3;
        }
        Player::ci.Draw(hdc, ptLeftTop.x, ptLeftTop.y, iWidth, iHeight, iFileWidth * iLeftIndex++, iFileHeight,
                        iFileWidth, iFileHeight);
    } else if (bPRight) {
        iUpDownIndex = 0;
        iLeftIndex = 0;
        if (iRightIndex == 8) {
            iRightIndex = 3;
        }
        Player::ci.Draw(hdc, ptLeftTop.x, ptLeftTop.y, iWidth, iHeight, iFileWidth * iRightIndex++, iFileHeight * 2,
                        iFileWidth, iFileHeight);
    } else if (bPUp) {
        iLeftIndex = 0;
        iRightIndex = 0;
        if (iUpDownIndex == 8) {
            iUpDownIndex = 3;
        }
        Player::ci.Draw(hdc, ptLeftTop.x, ptLeftTop.y, iWidth, iHeight, iFileWidth * iUpDownIndex++, 0, iFileWidth,
                        iFileHeight);
    } else if (bPDown) {
        iLeftIndex = 0;
        iRightIndex = 0;

        if (iUpDownIndex == 8) {
            iUpDownIndex = 3;
        }
        Player::ci.Draw(hdc, ptLeftTop.x, ptLeftTop.y, iWidth, iHeight, iFileWidth * iUpDownIndex++, 0, iFileWidth,
                        iFileHeight);
    } else {
        Player::ci.Draw(hdc, ptLeftTop.x, ptLeftTop.y, iWidth, iHeight, 0, 0, iFileWidth, iFileHeight);
    }
}
void Player::bulletStatusChange(bool stat) {
    if (stat) {
        BulletPlayer.bulletUsable = true;
    } else {
        BulletPlayer.bulletUsable = false;
        BulletPlayer.msLastShoot = tNow;
    }
}

Bullet *Player::getBullet() {
    return &BulletPlayer;
}

void Player::bulletSpawn() {
    POINT ptSpawn{ix, iy};
    BulletPlayer.ptPos.push_back(ptSpawn);
}

void Player::bulletMoving() {
    auto it = BulletPlayer.ptPos.begin();
    while (it < BulletPlayer.ptPos.end()) {
        if (it->y - iBulletSpeed > -BulletPlayer.iHeight) {
            it->y -= iBulletSpeed;
            ++it;
        } else {
            it = BulletPlayer.ptPos.erase(it);
        }
    }
}

// Enemy1
CImage &Enemy1::ci = ciEnemy1;

Enemy1::Enemy1() : Entity(0, 0, 40, 30, 15), BulletEnemy(ciEnemyBullet, 16, 16, 700, 8) {
    iHealth = 400;
    bDead = false;
    dir = -1;
}

void Enemy1::draw(HDC hdc) {
    Enemy1::ci.Draw(hdc, ptLeftTop.x, ptLeftTop.y, iWidth, iHeight, 0, 0, iFileWidth, iFileHeight);
}

void Enemy1::bulletStatusChange(bool stat) {
    if (stat) {
        BulletEnemy.bulletUsable = true;
    } else {
        BulletEnemy.bulletUsable = false;
        BulletEnemy.msLastShoot = tNow;
    }
}

Bullet *Enemy1::getBullet() {
    return &BulletEnemy;
}

void Enemy1::bulletSpawn() {
    POINT ptSpawn{ix, iy};
    BulletEnemy.ptPos.push_back(ptSpawn);
}

void Enemy1::bulletMoving() {
    auto it = BulletEnemy.ptPos.begin();
    while (it != BulletEnemy.ptPos.end()) {
        if (it->y + iEnemyBulletSpeed < BulletEnemy.iHeight + GHeight) {
            it->y += iEnemyBulletSpeed;
            ++it;
        } else {
            it = BulletEnemy.ptPos.erase(it);
        }
    }
}

bool Enemy1::isDead() {
    return bDead;
}

void Enemy1::changeDead() {
    bDead = true;
}

int Enemy1::getHealth() {
    return iHealth;
}

void Enemy1::changeHealth(int health) {
    iHealth += health;
}

void Enemy1::changeDir(int _dir) {
    dir = _dir;
}

int Enemy1::getDir() {
    return dir;
}

// Enemy2
CImage &Enemy2::ci = ciEnemy2;

Enemy2::Enemy2() : Entity(0, 0, 40, 30, 0), BulletEnemy(ciEnemyBullet2, 16, 16, 700, 8) {
    iHealth = 400;
    bDead = false;
    dir = -1;
}

void Enemy2::draw(HDC hdc) {
    Enemy2::ci.Draw(hdc, ptLeftTop.x, ptLeftTop.y, iWidth, iHeight, 0, 0, iFileWidth, iFileHeight);
}

void Enemy2::bulletStatusChange(bool stat) {
    if (stat) {
        BulletEnemy.bulletUsable = true;
    } else {
        BulletEnemy.bulletUsable = false;
        BulletEnemy.msLastShoot = tNow;
    }
}

Bullet *Enemy2::getBullet() {
    return &BulletEnemy;
}

void Enemy2::bulletSpawn() {
    POINT ptSpawn{ix, iy};
    BulletEnemy.ptPos.push_back(ptSpawn);
}

void Enemy2::bulletMoving() {
    auto it = BulletEnemy.ptPos.begin();
    while (it != BulletEnemy.ptPos.end()) {
        if (it->y + iEnemyBulletSpeed < BulletEnemy.iHeight + GHeight) {
            it->y += iEnemyBulletSpeed;
            ++it;
        } else {
            it = BulletEnemy.ptPos.erase(it);
        }
    }
}

bool Enemy2::isDead() {
    return bDead;
}

void Enemy2::changeDead() {
    bDead = true;
}

int Enemy2::getHealth() {
    return iHealth;
}

void Enemy2::changeHealth(int health) {
    iHealth += health;
}

void Enemy2::changeDir(int _dir) {
    dir = _dir;
}

int Enemy2::getDir() {
    return dir;
}
