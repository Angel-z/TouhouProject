#pragma once

#include <atlimage.h>

#include "entity.h"

extern HDC hdc;
extern HFONT normalFont;
extern HWND hwnd;
extern ULONGLONG tPre, tNow;

constexpr int ZOOM = 2;
constexpr int GWidth = 730;
constexpr int GHeight = 768;
constexpr int iBulletSpeed = 40;
constexpr int iEnemyBulletSpeed = 5;
constexpr int iMaxBulletDamage = 400;

extern CImage ciScreen;
extern RECT rect;

extern int iBackgroundOff;

// Sources
extern CImage ciBackground, ciTitleBk0, ciTitleBk, ciGameStart, ciBreak, ciLeftPanel, ciRightPanel, ciWin, ciFail,
    ciRetry, ciTitle;
extern CImage ciPlayer, ciPlayerBullet, ciEnemy1, ciEnemyBullet, ciEnemy2, ciEnemyBullet2;

// Entity Control
extern Player player;
extern std::vector<Entity *> EnemyExists;
extern std::vector<std::pair<Entity *, int>> EnemyDead;

extern unsigned long score;

// Game Control
extern bool running, win, fail;

// Player Acting
extern int iPlayerSpeed;
extern bool bPLeft, bPRight, bPUp, bPDown;
extern bool bShift;

// Mouse Check
extern int iMx, iMy;
extern bool LBdown;

// GameProgress
constexpr int Stage1_Loop = 3;
constexpr int Stage2_Loop = 6;
extern int GameTurn;
extern bool stage1ini, stage2ini;

// Func
void GamePaint(HDC hdc);
void GameCheck();
void MenuPaint(HDC hdc);
void MenuCheck();
void GameProgress();