#include <Windows.h>
#include <atlimage.h>

#include "globalVariable.h"
#include "tools.h"

void MyPaint(HDC hdc) {
    HDC mdc = CIscreen.GetDC();
    CIbackground.Draw(mdc, rect);
    CIplayer.Draw(mdc, 0, 0);
    CIscreen.ReleaseDC();
    CIscreen.Draw(hdc, rect);

    tPre = GetTickCount64();
}