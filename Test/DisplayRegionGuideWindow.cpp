
#include "DisplayRegionGuideWindow.hpp"


DisplayRegionGuideWindow::DisplayRegionGuideWindow(HWND arg_main_window_handle)
    : main_window_handle(arg_main_window_handle)
{
    if (this->main_window_handle == 0)
    {
        return;
    }
    HINSTANCE hInstance = this->GetInstanceHandle(this->main_window_handle);
    if (hInstance == 0)
    {
        return;
    }
    WNDCLASSEX wndclass;
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = (WNDPROC)DisplayRegionGuideWindow::WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
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

    if (! RegisterClassEx(&wndclass)) {
        return;
    }

    //...
    //this->createChildWindow();


}

DisplayRegionGuideWindow::~DisplayRegionGuideWindow()
{

}

void DisplayRegionGuideWindow::Show()
{
    if (this->this_window_handle == 0)
    {
        this->createChildWindow();
    }


}

void DisplayRegionGuideWindow::Update()
{
    MSG message;
    BOOL result;
    while ((result = ::GetMessage(&message, NULL, 0, 0)) != 0) {
        if (result == -1) {
            break;
        }
        else {
            ::TranslateMessage(&message);
            ::DispatchMessage(&message);
        }
    }


}



HINSTANCE DisplayRegionGuideWindow::GetInstanceHandle(HWND arg_window_handle) const
{
    return (HINSTANCE)GetWindowLongPtr(this->main_window_handle, GWLP_HINSTANCE);
}


bool DisplayRegionGuideWindow::createChildWindow()
{
    HINSTANCE hInstance = this->GetInstanceHandle(this->main_window_handle);
    if (hInstance == 0)
    {
        return false;
    }


    ////////////////////
    LPVOID lpMsgBuf;
    SetLastError(NO_ERROR);		//エラー情報をクリアする
    ///////////////////////
    //CreateWindowExW;
    //HWND hWnd;
    this->this_window_handle = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED,
        (LPCWSTR)this->class_name.c_str(),
        NULL,//L"透明ウィンドウテスト", //title name
        WS_CLIPCHILDREN | WS_CLIPSIBLINGS |
        WS_THICKFRAME, //window type
        0,    //x
        0,    //y
        300,    //width
        300,    //height
        this->main_window_handle, //parent window handle
        NULL, //menu handle
        hInstance, //instance handle
        NULL);
        //(LPVOID)this);  //static WNDPROC内ではthisポインタをキャストして使う.
    //////
    FormatMessage(				//エラー表示文字列作成
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    
    //MessageBox(NULL, (LPCWSTR)lpMsgBuf, NULL, MB_OK);	//メッセージ表示
    LocalFree(lpMsgBuf);
    ////////////////////////////


    if (this->this_window_handle)
    {
        ShowWindow(this->this_window_handle, SW_SHOW);
        UpdateWindow(this->this_window_handle);
        return true;
    }

    return false;
}

LRESULT CALLBACK DisplayRegionGuideWindow::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    int id;
    HDC hdc, hdc_mem;
    HBRUSH hBrush;
    PAINTSTRUCT ps;
    
    switch (msg) {
    case WM_CREATE:
        SetLayeredWindowAttributes(hWnd, RGB(255, 0, 0), 0, LWA_COLORKEY);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        hBrush = CreateSolidBrush(RGB(255, 0, 0));
        SelectObject(hdc, hBrush);
        ExtFloodFill(hdc, 1, 1, RGB(255, 255, 255), FLOODFILLSURFACE);
        hdc_mem = CreateCompatibleDC(hdc);
        DeleteDC(hdc_mem);
        SetBkMode(hdc, TRANSPARENT);
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