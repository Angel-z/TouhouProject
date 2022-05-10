#include <Windows.h>

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
extern void MyPaint(HDC hdc);

//HBITMAP hcharactor, hbackground;
HDC hdc, mdc, bufferdc;
HWND hwnd;
ULONGLONG tPre, tNow;

HBITMAP fullmap;

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
    hwnd = CreateWindow("Touhou", "Touhou Project", WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME, CW_USEDEFAULT, 0,
                        CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    if (!hwnd) {
        return FALSE;
    }

    MoveWindow(hwnd, 10, 10, 800, 600, false);
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    /////////////////////////////
    RECT rect;
    //BITMAP charactor{};

    GetClientRect(hwnd, &rect);
    hdc = GetDC(hwnd);
    mdc = CreateCompatibleDC(hdc);                                   //小DC，for大图
    bufferdc = CreateCompatibleDC(hdc);                              //小DC，for小图
    fullmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);  //大图

    SelectObject(mdc, fullmap);

    //hbackground = (HBITMAP)LoadImage(NULL, "di24.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
    //hcharactor = (HBITMAP)LoadImage(NULL, "ren24w.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
    //GetObject(hcharactor, sizeof(BITMAP), (LPVOID)&charactor);

    MyPaint(hdc);

    return TRUE;
}
