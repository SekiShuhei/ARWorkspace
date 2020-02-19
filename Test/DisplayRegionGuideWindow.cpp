
#include "DisplayRegionGuideWindow.hpp"


DisplayRegionSettingWindow::DisplayRegionSettingWindow(HWND arg_main_window_handle)
    : main_window_handle(arg_main_window_handle)
{
    
}

DisplayRegionSettingWindow::~DisplayRegionSettingWindow()
{
}

void DisplayRegionSettingWindow::Show()
{

}



ATOM DisplayRegionSettingWindow::initializeWindow(HINSTANCE hInstance)
{
    
    WNDCLASSEX wndclass;
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = (WNDPROC)DisplayRegionSettingWindow::WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;//インスタンス
    wndclass.hIcon = (HICON)LoadImage(NULL,
        MAKEINTRESOURCE(IDI_APPLICATION),
        IMAGE_ICON,
        0,
        0,
        LR_DEFAULTSIZE | LR_SHARED);
    wndclass.hCursor = (HCURSOR)LoadImage(NULL,
        MAKEINTRESOURCE(IDC_ARROW),
        IMAGE_CURSOR,
        0,
        0,
        LR_DEFAULTSIZE | LR_SHARED);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = (LPCWSTR)this->class_name.c_str();
    wndclass.hIconSm = (HICON)LoadImage(NULL, MAKEINTRESOURCE(IDI_APPLICATION),
        IMAGE_ICON,
        0, 0,
        LR_DEFAULTSIZE | LR_SHARED);

    return (RegisterClassEx(&wndclass));

}

bool DisplayRegionSettingWindow::createChildWindow()
{
    HINSTANCE hInstance = 
        (HINSTANCE)GetWindowLongPtr(this->main_window_handle, GWLP_HINSTANCE);
    if (hInstance == 0)
    {
        return false;
    }

    HWND hWnd;
    hWnd = CreateWindowEx(WS_EX_LAYERED,
        (LPCWSTR)this->class_name.c_str(),
        L"透明ウィンドウテスト", //title name
        WS_CHILD, //window type
        CW_USEDEFAULT,    //x
        CW_USEDEFAULT,    //y
        CW_USEDEFAULT,    //width
        CW_USEDEFAULT,    //height
        this->main_window_handle, //parent window handle
        NULL, //menu handle
        hInstance, //instance handle
        (LPVOID)this);  //static WNDPROC内ではthisポインタをキャストして使う.
    if (hWnd)
    {
        ShowWindow(hWnd, SW_SHOWNA);
        UpdateWindow(hWnd);
        return true;
    }

    return false;
}

LRESULT CALLBACK DisplayRegionSettingWindow::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    int id;
    HDC hdc, hdc_mem;
    HBRUSH hBrush;
    PAINTSTRUCT ps;
    //char szBuf[32] = "猫でもわかるLayer";
    //std::string text = "test";
    //BITMAP bmp_info;
    //HBITMAP hBmp;
    //int wx, wy;

    switch (msg) {
    case WM_CREATE:
        SetLayeredWindowAttributes(hWnd, RGB(255, 0, 0), 0, LWA_COLORKEY);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        hBrush = CreateSolidBrush(RGB(255, 0, 0));
        SelectObject(hdc, hBrush);
        ExtFloodFill(hdc, 1, 1, RGB(255, 255, 255), FLOODFILLSURFACE);
        //hBmp = (HBITMAP)LoadImage(hInst, "MYBMP", IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
        //GetObject(hBmp, (int)sizeof(BITMAP), &bmp_info);
        //wx = bmp_info.bmWidth;
        //wy = bmp_info.bmHeight;
        hdc_mem = CreateCompatibleDC(hdc);
        //SelectObject(hdc_mem, hBmp);
        //BitBlt(hdc, 0, 0, wx, wy, hdc_mem, 0, 0, SRCCOPY);
        //DeleteObject(hBmp);
        DeleteDC(hdc_mem);
        SetBkMode(hdc, TRANSPARENT);
        //TextOut(hdc, 10, 90, (LPCWSTR)text.c_str(), (int)strlen(szBuf));
        DeleteObject(hBrush);
        EndPaint(hWnd, &ps);
        break;
    case WM_CLOSE:
        id = MessageBox(hWnd,
            L"終了してもよろしいですか",
            L"確認",
            MB_YESNO | MB_ICONQUESTION);
        if (id == IDYES)
            DestroyWindow(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return (DefWindowProc(hWnd, msg, wp, lp));
    }
    return 0;
}