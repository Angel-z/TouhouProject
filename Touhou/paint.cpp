#include <Windows.h>
#include <atlimage.h>

#include "entity.h"
#include "globalVariable.h"
#include "tools.h"

void DrawLeftPanel(HDC mdc);
void DrawRightPanel(HDC mdc);

void GamePaint(HDC hdc) {
    HDC mdc = ciScreen.GetDC();
    SetStretchBltMode(mdc, HALFTONE);
    SetBrushOrgEx(mdc, 0, 0, NULL);

    // Draw LeftPanem
    DrawLeftPanel(mdc);

    // Draw RightPanel
    DrawRightPanel(mdc);

    // Done
    ciScreen.ReleaseDC();
    ciScreen.Draw(hdc, rect);

    tPre = GetTickCount64();
}

void MenuPaint(HDC hdc) {
    HDC mdc = ciScreen.GetDC();
    SetStretchBltMode(mdc, HALFTONE);
    SetBrushOrgEx(mdc, 0, 0, NULL);

    if (win || fail) {
        // Draw Background
        ciTitleBk.Draw(mdc, 0, 0, 1024, 768, 0, 0, ciTitleBk.GetWidth(), ciTitleBk.GetHeight());
        // Draw LeftPanel
        DrawLeftPanel(mdc);
        // Draw RightPanel
        DrawRightPanel(mdc);
        if (fail) {
            ciFail.Draw(mdc, GWidth / 2 - ciFail.GetWidth() * ZOOM / 2, 100, ciFail.GetWidth() * ZOOM,
                        ciFail.GetHeight() * ZOOM);
        } else {
            ciWin.Draw(mdc, GWidth / 2 - ciWin.GetWidth() / 2, 100, ciWin.GetWidth(), ciWin.GetHeight());
        }

        ciRetry.Draw(mdc, GWidth / 2 - ciRetry.GetWidth() / 2, GHeight / 2 - ciRetry.GetHeight() / 2,
                     ciRetry.GetWidth(), ciRetry.GetHeight());
    } else {
        ciTitleBk0.Draw(mdc, 0, 0, 1024, 768, 0, 0, ciTitleBk.GetWidth(), ciTitleBk.GetHeight());
        ciTitle.Draw(mdc, 30, 30);
        if (iMx > 50 && iMx < -50 + ciGameStart.GetWidth() / 2 && iMy > 400 && iMy < 400 + ciGameStart.GetHeight()) {
            SetCursor(::LoadCursor(nullptr, IDC_HAND));
            ciGameStart.Draw(mdc, 50, 400, ciGameStart.GetWidth() / 2, ciGameStart.GetHeight(),
                             ciGameStart.GetWidth() / 2, 0, ciGameStart.GetWidth() / 2, ciGameStart.GetHeight());
        } else {
            ciGameStart.Draw(mdc, 50, 400, ciGameStart.GetWidth() / 2, ciGameStart.GetHeight(), 0, 0,
                             ciGameStart.GetWidth() / 2, ciGameStart.GetHeight());
        }
    }

    // Done
    ciScreen.ReleaseDC();
    ciScreen.Draw(hdc, rect);
    tPre = GetTickCount64();
}

// Draw Panel
void DrawLeftPanel(HDC mdc) {
    // Draw LeftPanel
    HDC leftpanel = ciLeftPanel.GetDC();
    ciBackground.Draw(leftpanel, 0, iBackgroundOff);
    ciBackground.Draw(leftpanel, 0, 0, ciBackground.GetWidth(), iBackgroundOff, 0,
                      ciScreen.GetHeight() - iBackgroundOff, ciBackground.GetWidth(), iBackgroundOff);
    iBackgroundOff = (iBackgroundOff + 8) % GHeight;

    // Draw Enemy
    for (auto it = EnemyExists.begin(); it != EnemyExists.end(); ++it) {
        if (!(*it)->isDead()) {
            (*it)->draw(leftpanel);
        }
    }

    // Draw Break Effect
    for (auto it = EnemyDead.begin(); it != EnemyDead.end(); ++it) {
        if (it->second < 4) {
            ciBreak.Draw(leftpanel, it->first->ix - 32, it->first->iy - 32, 64, 64, it->second * 64, 0, 64, 64);
        } else {
            ciBreak.Draw(leftpanel, it->first->ix - 32, it->first->iy - 32, 64, 64, (it->second % 4) * 64, 64, 64, 64);
        }
        ++(it->second);
    }

    // Draw Player Bullet
    player.BulletPlayer.draw(leftpanel);

    // Draw Player
    player.draw(leftpanel);  // Last draw
    if (bShift) {
        ciEnemyBullet2.Draw(leftpanel, player.ix - player.iRadius, player.iy - player.iRadius, player.iRadius * 2,
                            player.iRadius * 2, 0, 0, 16, 16);
    }

    // Draw Enemy Bullet
    for (auto it = EnemyExists.begin(); it != EnemyExists.end(); ++it) {
        (*it)->getBullet()->draw(leftpanel);
    }

    // LeftPanel Done
    ciLeftPanel.ReleaseDC();
    ciLeftPanel.Draw(mdc, 0, 0);
}

void DrawRightPanel(HDC mdc) {
    HDC rightpanel = ciRightPanel.GetDC();
    SetBkMode(rightpanel, TRANSPARENT);
    SelectObject(rightpanel, normalFont);

    // Draw BackGround
    ciTitleBk.Draw(rightpanel, 0, 0, ciRightPanel.GetWidth(), ciRightPanel.GetHeight(),
                   ciTitleBk.GetWidth() - ciRightPanel.GetWidth(), 0, ciRightPanel.GetWidth(), ciTitleBk.GetHeight());

    // Draw Charactor
    ciCharactor.Draw(rightpanel, 0, 200, int(ciCharactor.GetWidth() / 1.7), int(ciCharactor.GetHeight() / 1.7));

    // Draw Text
    CString cscore, cst;
    cst.Format(_T("SCORE"));
    cscore.Format(_T("%06d"), score);
    SIZE csize;
    GetTextExtentPoint32(rightpanel, cst, cst.GetLength(), &csize);
    TextOut(rightpanel, ciRightPanel.GetWidth() / 2 - csize.cx / 2, 50, cst, cst.GetLength());
    GetTextExtentPoint32(rightpanel, cscore, cscore.GetLength(), &csize);
    TextOut(rightpanel, ciRightPanel.GetWidth() / 2 - csize.cx / 2, 120, cscore, cscore.GetLength());
    DeleteObject(normalFont);

    // RightPanel Done
    ciRightPanel.ReleaseDC();
    ciRightPanel.Draw(mdc, GWidth, 0);
}