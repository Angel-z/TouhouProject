#include "entity.h"

#include <atlimage.h>

#include "globalVariable.h"

Entity::Entity() {
    ix = 0;
    iy = 0;
    iFileWidth = 0;
    iFileHeight = 0;
    iWidth = 0;
    iHeight = 0;
    iRadius = 0;
    ptLeftTop = {0, 0};
}

Entity::Entity(int _x, int _y, int _iFileWidth, int _iFileHeight, int _iRadius) {
    ix = _x;
    iy = _y;
    iFileWidth = _iFileWidth;
    iFileHeight = _iFileHeight;
    iWidth = iFileWidth * ZOOM;
    iHeight = iFileHeight * ZOOM;
    iRadius = _iRadius;
    ptLeftTop.x = ix - iWidth / 2;
    ptLeftTop.y = iy - iHeight / 2;
}

void Entity::draw(HDC hdc, int x, int y) {}

CImage &Player::ci = ciPlayer;

Player::Player() : Entity(0, 0, 32, 48, 0) {
    ix = GWidth / 2 - iWidth / 2;
    iy = GHeight - iHeight + iHeight / 2;
    ptLeftTop.x = ix - iWidth / 2;
    ptLeftTop.y = iy - iHeight / 2;

    iUpDownIndex = 0;
    iLeftIndex = 0;
    iRightIndex = 0;
}

void Player::xChange(int x) {
    ix += x;
    ptLeftTop.x += x;
}

void Player::yChange(int y) {
    iy += y;
    ptLeftTop.y += y;
}

void Player::draw(HDC hdc, int x, int y) {
    if (bPLeft) {
        iUpDownIndex = 0;
        iRightIndex = 0;
        if (iLeftIndex == 8) {
            iLeftIndex = 3;
        }
        Player::ci.Draw(hdc, x, y, iWidth, iHeight, iFileWidth * iLeftIndex++, iFileHeight, iFileWidth, iFileHeight);
    } else if (bPRight) {
        iUpDownIndex = 0;
        iLeftIndex = 0;
        if (iRightIndex == 8) {
            iRightIndex = 3;
        }
        Player::ci.Draw(hdc, x, y, iWidth, iHeight, iFileWidth * iRightIndex++, iFileHeight * 2, iFileWidth, iFileHeight);
    } else if (bPUp) {
        iLeftIndex = 0;
        iRightIndex = 0;
        if (iUpDownIndex == 8) {
            iUpDownIndex = 3;
        }
        Player::ci.Draw(hdc, x, y, iWidth, iHeight, iFileWidth * iUpDownIndex++, 0, iFileWidth, iFileHeight);
    } else if (bPDown) {
        iLeftIndex = 0;
        iRightIndex = 0;

        if (iUpDownIndex == 8) {
            iUpDownIndex = 3;
        }
        Player::ci.Draw(hdc, x, y, iWidth, iHeight, iFileWidth * iUpDownIndex++, 0, iFileWidth, iFileHeight);
    } else {
        Player::ci.Draw(hdc, x, y, iWidth, iHeight, 0, 0, iFileWidth, iFileHeight);
    }
}
