#pragma once

extern HDC hdc;
extern HWND hwnd;
extern ULONGLONG tPre, tNow;

extern CImage CIscreen;
extern RECT rect;

void MyPaint(HDC hdc);

extern CImage CIbackground, CIplayer;
