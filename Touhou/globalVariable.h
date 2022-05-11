#pragma once

#include <atlimage.h>

#include "entity.h"

extern HDC hdc;
extern HWND hwnd;
extern ULONGLONG tPre, tNow;

constexpr int GWidth = 730;
constexpr int GHeight = 768;
constexpr int iPlayerSpeed = 10;

extern CImage ciScreen;
extern RECT rect;

// Sources
extern CImage ciBackground, ciTitleBk, ciPlayer;
extern Player player;

extern int iBackgroundOff;
constexpr int ZOOM = 2;

// Player Acting
extern bool bPLeft, bPRight, bPUp, bPDown;

// Func
void GameCheck();
void MyPaint(HDC hdc);
