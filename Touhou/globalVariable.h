#pragma once

#include <atlimage.h>

#include "entity.h"

extern HDC hdc;
extern HWND hwnd;
extern ULONGLONG tPre, tNow;

constexpr int ZOOM = 2;
constexpr int GWidth = 730;
constexpr int GHeight = 768;
constexpr int iPlayerSpeed = 15;
constexpr int iBulletSpeed = 35;
constexpr int iMaxBulletDamage = 400;

extern CImage ciScreen;
extern RECT rect;

extern int iBackgroundOff;


// Sources
extern CImage ciBackground, ciTitleBk, ciPlayer, ciPlayerBullet, ciEnemy1, ciEnemyBullet;
extern CImage ciEnemy2, ciEnemyBullet2;

// Entity
extern Player player;
extern std::vector<Entity *> EnemyExists;

// Player Acting
extern bool bPLeft, bPRight, bPUp, bPDown;
extern bool bShoot;

// Func
void GameCheck();
void MyPaint(HDC hdc);
