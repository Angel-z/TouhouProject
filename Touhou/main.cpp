#include <Windows.h>
#include <atlimage.h>

#include <vector>

#include "entity.h"
#include "globalVariable.h"
#include "resource.h"
#include "tools.h"

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HDC hdc;
HFONT normalFont;
HWND hwnd;
ULONGLONG tPre, tNow;

HINSTANCE hins;

CImage ciScreen;
RECT rect;

CImage ciBkBlock, ciBackground, ciTitleBk0, ciTitleBk, ciGameStart, ciBreak, ciLeftPanel, ciRightPanel, ciWin, ciFail,
    ciRetry, ciTitle, ciCharactor;
CImage ciPlayer, ciPlayerBullet, ciEnemy1, ciEnemyBullet, ciEnemy2, ciEnemyBullet2;

// Game Control
bool running = false, win = false, fail = false;

// Entity Control
Player player;
std::vector<Entity *> EnemyExists;
std::vector<std::pair<Entity *, int>> EnemyDead;

unsigned long score = 0;

int iBackgroundOff = 1;
int iPlayerSpeed = 15;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR szCmdLine, _In_ int iCmdShow) {
    MSG msg;

    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, iCmdShow)) {
        return FALSE;
    }

    hins = hInstance;

    GetMessage(&msg, NULL, NULL, NULL);
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            tNow = GetTickCount64();
            if (tNow - tPre >= 17) {
                if (running) {
                    GameProgress();
                    GamePaint(hdc);
                    GameCheck();
                } else {
                    MenuPaint(hdc);
                    MenuCheck();
                }
            }
        }
    }
    return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEX wcex{};

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = (WNDPROC)WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = NULL;
    wcex.hCursor = NULL;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "Touhou";
    wcex.hIconSm = NULL;

    return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
    hwnd = CreateWindow("Touhou", "Touhou Project", WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,
                        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    if (!hwnd) {
        return FALSE;
    }

    RECT rcWindow;
    RECT rcClient;
    int borderWidth, borderHeight;

    GetWindowRect(hwnd, &rcWindow);

    GetClientRect(hwnd, &rcClient);

    borderWidth = (rcWindow.right - rcWindow.left) - (rcClient.right - rcClient.left);
    borderHeight = (rcWindow.bottom - rcWindow.top) - (rcClient.bottom - rcClient.top);

    MoveWindow(hwnd, 0, 0, 1024, 768, true);
    SetWindowPos(hwnd, 0, 0, 0, borderWidth + 1024, borderHeight + 768, SWP_NOMOVE | SWP_NOZORDER);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    /////////////////////////////

    GetClientRect(hwnd, &rect);
    hdc = GetDC(hwnd);
    ciScreen.Create(1024, 768, 32);

    // resources load
    LoadImg(ciBkBlock, IDB_BkBlock);
    LoadImg(ciPlayer, IDB_Player);
    LoadImg(ciTitleBk0, IDB_TitleBk0);
    LoadImg(ciTitleBk, IDB_TitleBk);
    LoadImg(ciGameStart, IDB_GameStart);
    LoadImg(ciBreak, IDB_Break);
    LoadImg(ciPlayerBullet, IDB_PlayerBullet);
    LoadImg(ciEnemy1, IDB_Enemy1);
    LoadImg(ciEnemyBullet, IDB_EnemyBullet);
    LoadImg(ciEnemy2, IDB_Enemy2);
    LoadImg(ciEnemyBullet2, IDB_EnemyBullet2);
    LoadImg(ciWin, IDB_Win);
    LoadImg(ciFail, IDB_Fail);
    LoadImg(ciRetry, IDB_Retry);
    LoadImg(ciTitle, IDB_Title);
    LoadImg(ciCharactor, IDB_Charactor);

    ciLeftPanel.Create(GWidth, GHeight, 32);
    ciRightPanel.Create(rect.right - GWidth, GHeight, 32);
    ciBackground.Create(GWidth, 1024, 32);

    HDC hDcBackground = ciBackground.GetDC();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            ciBkBlock.Draw(hDcBackground, i * 256, j * 256);
        }
    }
    ciBkBlock.Destroy();
    ciBackground.ReleaseDC();

    // Font
    normalFont = CreateFont(45, 32, 0, 0, FW_MEDIUM, false, false, false, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
                            CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, "normal");

    return TRUE;
}
