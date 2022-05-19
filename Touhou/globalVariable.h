#pragma once

#include <atlimage.h>

#include "entity.h"

extern HDC hdc;
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
extern CImage ciBackground, ciTitleBk0, ciTitleBk, ciGameStart, ciBreak, ciLeftPanel;
extern CImage ciPlayer, ciPlayerBullet, ciEnemy1, ciEnemyBullet, ciEnemy2, ciEnemyBullet2;

// Entity Control
extern Player player;
extern std::vector<Entity *> EnemyExists;
extern std::vector<std::pair<Entity *, int>> EnemyDead;

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
constexpr ULONGLONG Stage1_Time = 2 * 60 * 1000;
constexpr ULONGLONG Stage2_Time = 2 * Stage1_Time;
constexpr ULONGLONG Stage3_Time = 3 * Stage1_Time;
constexpr ULONGLONG Stage4_Time = 4 * Stage1_Time;
extern ULONGLONG tGameStart, tGameNow;
extern bool stage1ini, stage2ini, stage3ini, stage4ini;

// Func
void GamePaint(HDC hdc);
void GameCheck();
void MenuPaint(HDC hdc);
void MenuCheck();
void GameProgress();