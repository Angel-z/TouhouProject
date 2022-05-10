#include <Windows.h>
#include <atlimage.h>

#include "globalVariable.h"
#include "tools.h"

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HDC hdc;
HWND hwnd;
ULONGLONG tPre, tNow;

CImage CIscreen;
RECT rect;

CImage CIbackground, CIplayer;

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
            if (tNow - tPre >= 60)
                MyPaint(hdc);
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

    MoveWindow(hwnd, 10, 10, 1024, 768, false);
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    /////////////////////////////

    GetClientRect(hwnd, &rect);
    hdc = GetDC(hwnd);
    CIscreen.Create(rect.right, rect.bottom, 32);

    // hbackground = (HBITMAP)LoadImage(NULL, "di24.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
    // hcharactor = (HBITMAP)LoadImage(NULL, "ren24w.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
    // GetObject(hcharactor, sizeof(BITMAP), (LPVOID)&charactor);

    CString cs_bk = "resources/bk.jpg";
    CString cs_player = "resources/1.png";
    LoadImg(CIbackground, cs_bk);
    LoadImg(CIplayer, cs_player);

    MyPaint(hdc);

    return TRUE;
}
