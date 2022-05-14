#include <Windows.h>
#include <atlimage.h>
#include <vector>

#include "entity.h"
#include "globalVariable.h"
#include "tools.h"

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HDC hdc;
HWND hwnd;
ULONGLONG tPre, tNow;

CImage ciScreen;
RECT rect;

CImage ciBkBlock, ciBackground, ciTitleBk, ciPlayer, ciPlayerBullet, ciEnemy1, ciEnemyBullet;

// Entity Control
Player player;
std::vector<Entity *> EnemyExists;

int iBackgroundOff = 1;


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR szCmdLine, _In_ int iCmdShow) {
    MSG msg;

    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, iCmdShow)) {
        return FALSE;
    }

    GetMessage(&msg, NULL, NULL, NULL);
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            tNow = GetTickCount64();
            if (tNow - tPre >= 17) {
                GameCheck();
                MyPaint(hdc);
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
    CString csBkBlock = "resources/background.png";
    CString csPlayer = "resources/pl00.png";
    CString csTitleBk = "resources/title_bk01.png";
    CString csPlayerBullet = "resources/pl00bullet.png";
    CString csEnemy1 = "resources/enemy1.png";
    CString csEnemyBullet = "resources/bullet1.png";

    LoadImg(ciBkBlock, csBkBlock);
    LoadImg(ciPlayer, csPlayer);
    LoadImg(ciTitleBk, csTitleBk);
    LoadImg(ciPlayerBullet, csPlayerBullet);
    LoadImg(ciEnemy1, csEnemy1);
    LoadImg(ciEnemyBullet, csEnemyBullet);

    ciBackground.Create(GWidth, 1024, 32);

    HDC hDcBackground = ciBackground.GetDC();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            ciBkBlock.Draw(hDcBackground, i * 256, j * 256);
        }
    }
    ciBkBlock.Destroy();
    ciBackground.ReleaseDC();


    //test
    Enemy1 *e1 = new Enemy1();
    e1->xySet(100, 100);
    EnemyExists.push_back(e1);

    MyPaint(hdc);

    return TRUE;
}
